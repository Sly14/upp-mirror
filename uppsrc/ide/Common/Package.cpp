#include "Common.h"

bool IsUppValueChar(int c)
{
	return c > ' ' && c != ',' && c != ';';
}

String ReadValue(CParser& p)
{
	p.Spaces();
	if(p.IsString())
		return p.ReadOneString();
	StringBuffer v;
	while(IsUppValueChar(p.PeekChar()))
		v.Cat(p.GetChar());
	p.Spaces();
	return v;
}

static bool sMatchOr(CParser& p, const Vector<String>& flag);

static bool sMatchFlag(CParser& p, const Vector<String>& flag)
{
	if(p.Char('!'))
		return !sMatchFlag(p, flag);
	if(p.Char('(')) {
		bool b = sMatchOr(p, flag);
		p.PassChar(')');
		return b;
	}
	if(p.IsEof())
		return true;
	return FindIndex(flag, p.ReadId()) >= 0;
}

static bool sMatchAnd(CParser& p, const Vector<String>& flag)
{
	bool b = sMatchFlag(p, flag);
	while(p.IsId() || p.IsChar('!') || p.IsChar('(') || p.Char2('&', '&') || p.Char('&'))
		b = sMatchFlag(p, flag) && b;
	return b;
}

static bool sMatchOr(CParser& p, const Vector<String>& flag)
{
	bool b = sMatchAnd(p, flag);
	while(p.Char2('|', '|') || p.Char('|'))
		b = sMatchFlag(p, flag) || b;
	return b;
}

bool MatchWhen_X(const String& when, const Vector<String>& flag)
{
	CParser p(when);
	bool b = sMatchOr(p, flag);
	if(!p.IsEof())
		p.ThrowError("expected end of expression");
	return b;
}

bool MatchWhen(const String& when, const Vector<String>& flag)
{
	try {
		return MatchWhen_X(when, flag);
	}
	catch(CParser::Error e) {
		PutConsole(String().Cat()
		           << "Invalid When expression: " << AsCString(when) << ": " << e);
		return false;
	}
}

String ReadWhen(CParser& p) {
	String when;
	if(p.Char('('))
		if(p.IsString())
			when = p.ReadString();
		else {
			const char *b = p.GetPtr();
			int lvl = 0;
			for(;;) {
				if(p.IsEof() || lvl == 0 && p.IsChar(')'))
					break;
				if(p.Char('('))
					lvl++;
				else
				if(p.Char(')'))
					lvl--;
				else
					p.SkipTerm();
			}
			when = String(b, p.GetPtr());
			p.Char(')');
		}
	return when;
}

String AsStringWhen(const String& when) {
	String out;
	try {
		Vector<String> x;
		MatchWhen_X(when, x);
		out << '(' << when << ')';
	}
	catch(CParser::Error) {
		out << '(' << AsCString(when) << ')';
	}
	return out;
}

String CustomStep::AsString() const {
	return "custom" + AsStringWhen(when) + ' ' + AsCString(ext) + ",\n\t" +
					  AsCString(command, 70, "\t") + ",\n\t" +
					  AsCString(output, 70, "\t") + ";\n\n";
}

void CustomStep::Load(CParser& p) throw(CParser::Error) {
	when = ReadWhen(p);
	ext = p.ReadString();
	p.PassChar(',');
	command = p.ReadString();
	p.PassChar(',');
	output = p.ReadString();
	p.PassChar(';');
}

String CustomStep::GetExt() const {
	return ext[0] != '.' ? "." + ToLower(ext) : ToLower(ext);
}

bool   CustomStep::MatchExt(const char *fn) const {
	return ToLower(GetFileExt(fn)) == GetExt();
}

bool LoadOpt(CParser& p, const char *key, Array<OptItem>& v) {
	if(p.Id(key)) {
		String when = ReadWhen(p);
		do {
			OptItem& m = v.Add();
			m.when = when;
			m.text = ReadValue(p);
		}
		while(p.Char(','));
		return true;
	}
	return false;
}

bool LoadFOpt(CParser& p, const char *key, Array<OptItem>& v) {
	if(p.Id(key)) {
		OptItem& m = v.Add();
		m.when = ReadWhen(p);
		m.text = ReadValue(p);
		return true;
	}
	return false;
}

Package::Package()
{
	charset = 0;
	optimize_speed = false;
	noblitz = true;
}

void Package::Load(const char *path) {
	for(;;) {
		charset = 0;
		optimize_speed = false;
		noblitz = false;
		library.Clear();
		target.Clear();
		flag.Clear();
		option.Clear();
		link.Clear();
		uses.Clear();
		accepts.Clear();
		file.Clear();
		config.Clear();
		custom.Clear();
		unknown.Clear();
		description.Clear();
		String f = LoadFile(path);
		time = FileGetTime(path);
		CParser p(f);
		try {
			while(!p.IsEof()) {
				if(!LoadOpt(p, "options", option) &&
				   !LoadOpt(p, "link", link) &&
				   !LoadOpt(p, "library", library) &&
				   !LoadOpt(p, "flags", flag) &&
				   !LoadOpt(p, "target", target) &&
				   !LoadOpt(p, "uses", uses))
				if(p.Id("charset"))
					charset = CharsetByName(p.ReadString());
				else
				if(p.Id("description"))
					description = p.ReadString();
				else
				if(p.Id("acceptflags")) {
					do
						accepts.Add(ReadValue(p));
					while(p.Char(','));
				}
				else
				if(p.Id("noblitz"))
				   noblitz = true;
				else
				if(p.Id("optimize_speed"))
					optimize_speed = true;
				else
				if(p.Id("optimize_size"))
					optimize_speed = false;
				else
				if(p.Id("file")) {
					do {
						file.Add(ReadValue(p));
						while(!p.IsChar(',') && !p.IsChar(';')) {
							if(!LoadFOpt(p, "options", file.Top().option) &&
							   !LoadFOpt(p, "depends", file.Top().depends))
							if(p.Id("optimize_speed"))
								file.Top().optimize_speed = true;
							else
							if(p.Id("optimize_size"))
								file.Top().optimize_speed = false;
							else
							if(p.Id("readonly"))
								file.Top().readonly = true;
							else
							if(p.Id("separator"))
								file.Top().separator = true;
							else
							if(p.Id("charset"))
								file.Top().charset = CharsetByName(p.ReadString());
							else
							if(p.Id("tabsize"))
								file.Top().tabsize = minmax(p.ReadInt(), 1, 20);
							else
							if(p.Id("font"))
								file.Top().font = minmax(p.ReadInt(), 0, 3);
							else
							if(p.Id("highlight"))
								file.Top().highlight = p.ReadId();
							else {
								const char *q = p.GetPtr();
								if(p.Char('('))
									while(!p.Char(')'))
										p.SkipTerm();
								else
									p.SkipTerm();
								file.Top().unknown.Cat(' ');
								file.Top().unknown.Cat(q, int(p.GetPtr() - q));
							}
						}
					}
					while(p.Char(','));
				}
				else
				if(p.Id("mainconfig")) {
					do {
						String c = p.ReadString();
						p.Char('=');
						p.Id("external"); // Backward compatibility...
						p.Id("console");
						p.Id("remotelinux");
						p.Id("normal");
						String f = p.ReadString();
						Config& cf = config.Add();
						cf.name = c;
						cf.param = f;
					}
					while(p.Char(','));
				}
				else
				if(p.Id("custom"))
					custom.Add().Load(p);
				else {
					unknown << p.ReadId() << " ";
					const char *q = p.GetPtr();
					while(!p.Char(';') && !p.IsEof())
						p.SkipTerm();
					unknown.Cat("\r\n\r\n");
					unknown.Cat(q, p.GetPtr());
				}
				p.Char(';');
			}
			return;
		}
		catch(CParser::Error error) {
		prompt:
			switch(Prompt(Ctrl::GetAppName(), CtrlImg::exclamation(),
			              error + "&while parsing package " + DeQtf(path),
				          "Edit \\& Retry", "Ignore",  "Stop")) {
			case 0:
				if(!PromptYesNo("Ignoring will damage package. Everything past the "
					            "point of error will be lost.&Do you want to continue ?"))
					goto prompt;
				Save(path);
				return;
			case 1: {
					TopWindow win;
					LineEdit edit;
					edit.Set(f);
					edit.SetCursor(edit.GetPos(p.GetLine() - 1));
					win.Title(path);
					win.Add(edit.SizePos());
					win.Run();
					SaveFile(path, edit.Get());
				}
				break;
			case -1:
				exit(1);
			}
		}
	}
}

String WriteValue(const String& x) {
	for(const char *s = x; s < x.End(); s++)
		if(!IsUppValueChar(*s))
			return AsCString(x);
	return x;
}

void putopt(Stream& out, const char *key, const Array<OptItem>& m) {
	bool was = false;
	for(int i = 0; i < m.GetCount(); i++)
		if(IsNull(m[i].when)) {
			if(was)
				out << ",\n\t";
			else
				out << key << "\n\t";
			out << WriteValue(m[i].text);
			was = true;
		}
	if(was)
		out << ";\n\n";
	for(int i = 0; i < m.GetCount(); i++)
		if(!IsNull(m[i].when))
			out << key << AsStringWhen(m[i].when) << ' ' << WriteValue(m[i].text) << ";\n\n";
}

void putp(Stream& out, const char *key, const Vector<String>& v)
{
	if(v.GetCount()) {
		out << key << "\n";
		for(int i = 0; i < v.GetCount(); i++) {
			if(i) out << ",\n";
			out << '\t' << WriteValue(v[i]);
		}
		out << ";\n\n";
	}
}

void putfopt(Stream& out, const char *key, const Array<OptItem>& m)
{
	for(int i = 0; i < m.GetCount(); i++)
		out << "\n\t\t" << key << AsStringWhen(m[i].when) << ' ' << WriteValue(m[i].text);
}

bool Package::Save(const char *path) const {
	StringStream out;
	if(description.GetCount())
		out << "description " << AsCString(description) << ";\n\n";
	if(charset > 0 && charset < CharsetCount() || charset == CHARSET_UTF8)
		out << "charset " << AsCString(CharsetName(charset)) << ";\n\n";
	if(optimize_speed)
		out << "optimize_speed;\n\n";
	if(noblitz)
		out << "noblitz;\n\n";
	putp(out, "acceptflags", accepts);
	putopt(out, "flags", flag);
	putopt(out, "uses", uses);
	putopt(out, "target", target);
	putopt(out, "library", library);
	putopt(out, "options", option);
	putopt(out, "link", link);
	if(file.GetCount()) {
		out << "file\n";
		int i;
		for(i = 0; i < file.GetCount(); i++) {
			if(i) out << ",\n";
			const File& f = file[i];
			out << '\t' << WriteValue(f);
			if(f.readonly)
				out << " readonly";
			if(f.separator)
				out << " separator";
			if(f.tabsize > 0)
				out << " tabsize " << f.tabsize;
			if(f.font > 0)
				out << " font " << f.font;
			if(f.optimize_speed)
				out << " optimize_speed";
			if(f.charset > 0 && f.charset < CharsetCount() || f.charset == CHARSET_UTF8)
				out << " charset " << AsCString(CharsetName(f.charset));
			if(!IsNull(f.highlight))
				out << " highlight " << f.highlight;
			putfopt(out, "options", f.option);
			putfopt(out, "depends", f.depends);
			out << f.unknown;
		}
		out << ";\n\n";
	}
	if(config.GetCount()) {
		out << "mainconfig\n";
		for(int i = 0; i < config.GetCount(); i++) {
			const char *o[] = {
				"normal", "remote", "external", "console"
			};
			const Config& f = config[i];
			if(i) out << ",\n";
			out << '\t' << AsCString(f.name) << " = " << AsCString(f.param);
		}
		out << ";\n\n";
	}
	for(int i = 0; i < custom.GetCount(); i++)
		out << custom[i].AsString();
	out << unknown;
	return SaveChangedFile(path, out.GetResult());
}
