#include "CppBase.h"

NAMESPACE_UPP

#define LTIMING(x)  RTIMING(x)
#define LLOG(x)     // LOG(x)

bool IsCPPFile(const String& path)
{
	return findarg(ToLower(GetFileExt(path)) , ".c", ".cpp", ".cc" , ".cxx", ".icpp") >= 0;
}

void SetSpaces(String& l, int pos, int count)
{
	StringBuffer s = l;
	memset(~s + pos, ' ', count);
	l = s;
}

void RemoveComments(String& l, bool& incomment)
{
	int q = -1;
	int w = -1;
	if(incomment)
		q = w = 0;
	else {
		q = l.Find("/*");
		if(q >= 0)
			w = q + 2;
	}
	while(q >= 0) {
		int eq = l.Find("*/", w);
		if(eq < 0) {
			incomment = true;
			SetSpaces(l, q, l.GetCount() - q);
			return;
		}
		SetSpaces(l, q, eq + 2 - q);
		incomment = false;
		q = l.Find("/*");
		w = q + 2;
	}
}

static VectorMap<String, PPMacro> sAllMacros;
static ArrayMap<String, PPFile>   sPPfile;
static int                        sPPserial;

void SerializePPFiles(Stream& s)
{
	s % sAllMacros % sPPfile % sPPserial;
}

void CleanPP()
{
	sAllMacros.Clear();
	sPPfile.Clear();
	sPPserial = 0;
}

void SweepPPFiles(const Index<String>& keep)
{
	for(int i = 0; i < sPPfile.GetCount(); i++)
		if(keep.Find(sPPfile.GetKey(i)) < 0 && !sPPfile.IsUnlinked(i))
			sPPfile.Unlink(i);
}

String GetSegmentFile(int segment_id)
{
	for(int i = 0; i < sPPfile.GetCount(); i++) {
		const Array<PPItem>& m = sPPfile[i].item;
		for(int j = 0; j < m.GetCount(); j++)
			if(m[j].type == PP_DEFINES && m[j].segment_id == segment_id)
				return sPPfile.GetKey(i);
	}
	return "<not found>";
}

PPMacro *FindPPMacro(const String& id, Index<int>& segment_id, int& segmenti)
{
	if(id == "_PREV_NAME") DDUMP(segment_id);
	Index<int> undef;
	PPMacro *r;

	for(int pass = 0; pass < 2; pass++) {
		r = NULL;
		int best = segmenti;
		int line = -1;
		int q = sAllMacros.Find(id);
		if(id == "_PREV_NAME") DDUMP(q);
		while(q >= 0) {
			PPMacro& m = sAllMacros[q];
			if(m.macro.IsUndef()) {
				if(pass == 0 && segment_id.Find(m.segment_id) >= 0) {
					undef.FindAdd(m.segment_id);
					if(id == "_PREV_NAME") DLOG("undef " << m.segment_id << ", line " << m.line);
				}
			}
			else
			if(pass == 0 || undef.Find(m.undef_segment_id) < 0) {
				int si = segment_id.Find(m.segment_id);
				if(id == "_PREV_NAME") DDUMP(m.segment_id), DDUMP(m.line), DDUMP(si);
				if(si > best || si >= 0 && si == best && m.line > line) {
					best = si;
					line = m.line;
					r = &m;
				}
			}
			q = sAllMacros.FindNext(q);
		}
		if(undef.GetCount() == 0)
			break;
	}
	return r;
}

const CppMacro *FindMacro(const String& id, Index<int>& segment_id, int& segmenti)
{
	PPMacro *m = FindPPMacro(id, segment_id, segmenti);
	return m ? &m->macro : NULL;
}

String GetAllMacros(const String& id, Index<int>& segment_id)
{
	String r;
	int q = sAllMacros.Find(id);
	while(q >= 0) {
		const PPMacro& m = sAllMacros[q];
		if(segment_id.Find(m.segment_id) >= 0)
			r << '\n' << m.macro;
		q = sAllMacros.FindNext(q);
	}
	return r;
}

void PPFile::CheckEndNamespace(Vector<int>& namespace_block, int level)
{
	if(namespace_block.GetCount() && namespace_block.Top() == level) {
		namespace_block.Drop();
		item.Add().type = PP_NAMESPACE_END;
	}
}

void PPFile::Parse(Stream& in)
{
	RTIMING("PPFile::Parse");
	for(int i = 0; i < ppmacro.GetCount(); i++)
		sAllMacros.Unlink(ppmacro[i]);
	ppmacro.Clear();
	item.Clear();
	includes.Clear();
	bool was_using = false;
	bool was_namespace = false;
	int  level = 0;
	bool incomment = false;
	Vector<int> namespace_block;
	bool next_segment = true;
	Index<int> local_segments;
	int linei = 0;
	while(!in.IsEof()) {
		String l = in.GetLine();
		while(*l.Last() == '\\' && !in.IsEof()) {
			l.Trim(l.GetLength() - 1);
			l.Cat(in.GetLine());
		}
		RemoveComments(l, incomment);
		try {
			CParser p(l);
			if(p.Char('#')) {
				if(p.Id("define")) {
					if(next_segment) {
						PPItem& m = item.Add();
						m.type = PP_DEFINES;
						m.segment_id = ++sPPserial;
						next_segment = false;
						local_segments.Add(sPPserial);
					}
					CppMacro def;
					String   id = def.Define(p.GetPtr());
					if(id.GetCount()) {
						PPMacro m;
						m.segment_id = sPPserial;
						m.line = linei;
						m.macro = def;
						ppmacro.Add(sAllMacros.Put(id, m));
					}
				}
				else
				if(p.Id("undef")) {
					if(p.IsId()) {
						String id = p.ReadId();
						int segmenti = -1;
						PPMacro *um = FindPPMacro(id, local_segments, segmenti);
						if(um) { // heuristic: only local undefs are allowed
							PPItem& m = item.Add();
							m.type = PP_DEFINES;
							m.segment_id = ++sPPserial;
							um->undef_segment_id = m.segment_id;
							next_segment = true;
							local_segments.Add(sPPserial);
							if(id.GetCount()) {
								PPMacro m;
								m.segment_id = sPPserial;
								m.line = linei;
								m.macro.SetUndef();
								ppmacro.Add(sAllMacros.Put(id, m));
							}
						}
					}
				}
				else
				if(p.Id("include")) {
					PPItem& m = item.Add();
					next_segment = true;
					m.type = PP_INCLUDE;
					m.text = TrimBoth(p.GetPtr());
					if(IsNull(m.text))
						item.Drop();
					else
						includes.FindAdd(m.text);
				}
			}
			else {
				while(!p.IsEof()) {
					if(was_namespace) {
						int type = was_using ? PP_USING : PP_NAMESPACE;
						String id;
						while(p.Char2(':', ':'))
							id = "::";
						id << p.ReadId();
						while(p.Char2(':', ':'))
							id << "::" << p.ReadId();
						if(!was_using)
							namespace_block.Add(level);
						if(!was_using || level == 0) {
							PPItem& m = item.Add();
							next_segment = true;
							m.type = type;
							m.text = id;
						}
						was_namespace = was_using = false;
					}
					else
					if(p.Id("using"))
						was_using = true;
					else
					if(p.Id("namespace"))
						was_namespace = true;
					else {
						was_using = was_namespace = false;
						if(p.IsId()) {
							static const VectorMap<String, String>& namespace_macro = GetNamespaceMacros();
							static const Index<String>& namespace_end_macro = GetNamespaceEndMacros();

							String id = p.ReadId();
							int q = namespace_macro.Find(id);
							if(q > 0) {
								PPItem& m = item.Add();
								next_segment = true;
								m.type = PP_NAMESPACE;
								m.text = namespace_macro[q];
								namespace_block.Add(level);
								level++;
							}
							else {
								q = namespace_end_macro.Find(id);
								if(q >= 0) {
									level--;
									CheckEndNamespace(namespace_block, level);
								}
							}
						}
						else
						if(p.Char('}')) {
							if(level > 0) {
								level--;
								CheckEndNamespace(namespace_block, level);
							}
						}
						else
						if(p.Char('{'))
							level++;
						else
							p.SkipTerm();
					}
				}
			}
		}
		catch(...) {}
		linei++;
	}
}

void PPFile::Dump() const
{
	for(int i = 0; i < item.GetCount(); i++) {
		const PPItem& m = item[i];
		String ll;
		ll << decode(m.type, PP_DEFINES, "#defines ", PP_INCLUDE, "#include ",
		                     PP_USING, "using namespace ", PP_NAMESPACE, "namespace ",
		                     PP_NAMESPACE_END, "}", "");
		if(m.type == PP_DEFINES)
			ll << m.segment_id;
		else
			ll << m.text;
		if(m.type == PP_NAMESPACE)
			ll << " {";
		LOG(ll);
	}
	LOG("----- includes:");
	DUMPC(includes);
}

static VectorMap<String, Time>     sPathFileTime;
static VectorMap<String, String>   sIncludePath;
static VectorMap<String, bool>     sIncludes;
static ArrayMap<String, PPFile>    sFlatPP;
static String                      sInclude_Path;

void PPSync(const String& include_path)
{
	sPathFileTime.Clear();
	sIncludePath.Clear();
	sIncludes.Clear();
	sFlatPP.Clear();
	sInclude_Path = include_path;
}

String GetIncludePath()
{
	return sInclude_Path;
}

String GetIncludePath0(const char *s, const char *filedir)
{
	RTIMING("GetIncludePath0");
	while(IsSpace(*s))
		s++;
	int type = *s;
	if(type == '<' || type == '\"' || type == '?') {
		s++;
		String name;
		if(type == '<') type = '>';
		while(*s != '\r' && *s != '\n') {
			if(*s == type) {
				if(type == '\"') {
					String fn = NormalizePath(name, filedir);
					if(FileExists(fn))
						return fn;
				}
				return GetFileOnPath(name, GetIncludePath(), false);
			}
			name.Cat(*s++);
		}
	}
	return Null;
}

Time GetFileTimeCached(const String& p)
{
	RTIMING("GetFileTimeCached");
	int q = sPathFileTime.Find(p);
	if(q >= 0)
		return sPathFileTime[q];
	Time m = FileGetTime(p);
	sPathFileTime.Add(p, m);
	return m;
}

String GetIncludePath(const String& s, const String& filedir)
{
	RTIMING("GetIncludePath");
	String key;
	{ RTIMING("GetIncludePath1");
	key << s << "#" << filedir << "#" << GetIncludePath();
	}
	{ RTIMING("GetIncludePath2");
	int q = sIncludePath.Find(key);
	if(q >= 0)
		return sIncludePath[q];
	}
	String p = GetIncludePath0(s, filedir);
	sIncludePath.Add(key, p);
	return p;
}

const PPFile& GetPPFile(const char *path)
{
	Time tm = GetFileTimeCached(path);
	PPFile& f = sPPfile.GetPut(path);
	if(f.filetime != tm) {
		f.filetime = tm;
		LTIMING("PP read");
		FileIn in(path);
		f.Parse(in);
	}
	return f;
}

bool IsSameFile(const String& f1, const String& f2)
{
	return NormalizePath(f1) == NormalizePath(f2);
}

bool IncludesFile(const String& parent_path, const String& path, Index<String>& visited)
{
	HITCOUNT("IncludesFile0");
	if(visited.Find(parent_path) >= 0)
		return false;
	visited.Add(parent_path);
	if(IsSameFile(parent_path, path))
		return true;
	const PPFile& f = GetPPFile(parent_path);
	for(int i = 0; i < f.includes.GetCount(); i++) {
		String key = path + "#" + f.includes[i];
		int q = sIncludes.Find(key);
		if(q >= 0) {
			HITCOUNT("IncludesFile cached");
			if(sIncludes[q])
				return true;
		}
		else {
			HITCOUNT("IncludesFile getpath");
			String p = GetIncludePath(f.includes[i], GetFileFolder(parent_path));
			bool   b = p.GetCount() && IncludesFile(p, path, visited);
			sIncludes.Add(key, b);
			if(b)
				return true;
		}
	}
	return false;
}

bool IncludesFile(const String& parent_path, const String& path)
{
	LTIMING("IncludesFile");
	Index<String> visited;
	return IncludesFile(parent_path, path, visited);
}

void CreateFlatPP(PPFile& fp, const char *path, Index<String>& visited)
{
	if(visited.Find(path) >= 0)
		return;
	visited.Add(path);
	LLOG("CreateFlatPP " << path << LOG_BEGIN);
	const PPFile& pp = GetPPFile(path);
	for(int i = 0; i < pp.item.GetCount(); i++) {
		const PPItem& m = pp.item[i];
		if(m.type == PP_INCLUDE) {
			String s = GetIncludePath(m.text, GetFileFolder(path));
			if(s.GetCount())
				CreateFlatPP(fp, s, visited);
		}
		else
			fp.item.Add(m);
	}
	LLOG(LOG_END);
}

const PPFile& GetFlatPPFile(const char *path, Index<String>& visited)
{
	LLOG("GetFlatPPFile " << path);
	int q = sFlatPP.Find(path);
	if(q >= 0)
		return sFlatPP[q];
	PPFile& fp = sFlatPP.Add(path);
	const PPFile& pp = GetPPFile(path);
	for(int i = 0; i < pp.item.GetCount(); i++) {
		const PPItem& m = pp.item[i];
		if(m.type == PP_INCLUDE) {
			String s = GetIncludePath(m.text, GetFileFolder(path));
			if(s.GetCount() && visited.Find(s) < 0) {
				visited.Add(s);
				const PPFile& pp = GetFlatPPFile(s, visited);
				for(int i = 0; i < pp.item.GetCount(); i++)
					fp.item.Add(pp.item[i]);
			}
		}
		else
			fp.item.Add(m);
	}
	return fp;
}

const PPFile& GetFlatPPFile(const char *path)
{
	Index<String> visited;
	visited.Add(path);
	return GetFlatPPFile(path, visited);
}

END_UPP_NAMESPACE
