#include "ide.h"

struct GatherLinksIterator : RichText::Iterator {
	Index<String> link;

	virtual bool operator()(int pos, const RichPara& para)
	{
		for(int i = 0; i < para.GetCount(); i++) {
			String l = para[i].format.link;
			if(!IsNull(l))
				link.FindAdd(l);
		}
		return false;
	}
};

void GatherLinks(Index<String>& link, const char *topic)
{
	if(link.Find(topic) < 0) {
		Topic p = GetTopic(topic);
		if(IsNull(p.text))
			return;
		link.Add(topic);
		RichText txt = ParseQTF(p.text);
		SyncTopicFile(txt, topic, ":ide:", p.title);
		GatherLinksIterator ti;
		txt.Iterate(ti);
		for(int i = 0; i < ti.link.GetCount(); i++)
			GatherLinks(link, ti.link[i]);
	}
}

Index<String> TopicCtrl::idelink;

void TopicCtrl::ScanDirForTpp(const char *dir, const String& rel,
                              Index<String>& donepackage, Index<String>& lang_list)
{
	TopicLink tl;
	for(FindFile pff(AppendFileName(dir, "*.*")); pff; pff.Next()) {
		if(pff.IsFolder() && *pff.GetName() != '.') {
			String pdir = AppendFileName(dir, pff.GetName());
			tl.package = rel + pff.GetName();
			if(donepackage.Find(tl.package) < 0) {
				donepackage.Add(tl.package);
				for(FindFile ff(AppendFileName(pdir, "*.tpp")); ff; ff.Next())
					if(ff.IsFolder()) {
						tl.group = GetFileTitle(ff.GetName());
						String dir = AppendFileName(pdir, ff.GetName());
						for(FindFile ft(AppendFileName(dir, "*.tpp")); ft; ft.Next())
							if(ft.IsFile()) {
								tl.topic = GetFileTitle(ft.GetName());
								int q = tl.topic.ReverseFind('$');
								String l;
								if(q >= 0) {
									l = ToUpper(tl.topic.Mid(q + 1));
									lang_list.FindAdd(l);
								}
								map.GetAdd(tl.package).GetAdd(tl.group).FindAdd(tl.topic);
							}
					}
			}
			ScanDirForTpp(pdir, tl.package + '/', donepackage, lang_list);
		}
	}
}

int map_serial, topic_serial;

void TopicCtrl::LoadMap()
{
	map.Clear();
	lang_list.Clear();
	Vector<String> upp = GetUppDirs();
	Index<String> donepackage, lang_list;
	for(int i = 0; i < upp.GetCount(); i++)
		ScanDirForTpp(upp[i], String(), donepackage, lang_list);
	Vector<String> l = lang_list.PickKeys();
	Sort(l);
	String lng = ~lang;
	lang.Clear();
	lang.Add("All");
	for(int i = 0; i < l.GetCount(); i++)
		lang.Add(l[i]);
	if(lng.GetCount() && lang.Find(lng))
		lang <<= lng;
	else
	if(lang.Find("EN-US"))
		lang <<= "EN-US";
	else
	if(lang.GetCount())
		lang.SetIndex(0);
}

const char *sTopicHome = "\3topic://ide/app/index$en-us";

void TopicCtrl::SyncDocTree()
{
	if(map_serial != topic_serial) {
		LoadMap();
		map_serial = topic_serial;
	}
	
	Vector<String> ss = Split((String)~search, ' ');
	Vector<String> sdx;
	for(int i = 0; i < ss.GetCount(); i++)
		sdx.Add(ToUtf8(ToLower(FromUtf8(ss[i]))));

	ClearTree();

	String hdx = sTopicHome + 1;
	if(idelink.GetCount() == 0)
		GatherLinks(idelink, hdx);
	int ide;
	bool idefirst = true;
	if(MatchTopicLink(hdx, sdx)) {
		ide = AddTree(0, IdeImg::Package(), "\3" + hdx, "TheIDE help");
		idefirst = false;
	}
	for(int i = 0; i < idelink.GetCount(); i++) {
		if(idelink[i] != hdx && MatchTopicLink(idelink[i], sdx)) {
			if(idefirst) {
				ide = AddTree(0, IdeImg::Package(), "\3" + hdx, "TheIDE help");
				idefirst = false;
			}
			AddTree(ide, TopicImg::Topic(), "\3" + idelink[i], GetTopic(idelink[i]).title);
		}
	}

	Index<String> used;
	const Workspace& wspc = GetIdeWorkspace();
	for(int i = 0; i < wspc.GetCount(); i++)
		used.Add(wspc[i]);

	int usid;
	bool usedfirst = true;
	int otid;
	bool otherfirst = true;

	String lng = ~lang;
	for(int i = 0; i < map.GetCount(); i++) {
		TopicLink tl;
		tl.package = map.GetKey(i);
		bool packagefirst = true;
		int pid;
		VectorMap<String, Index<String> >& group = map[i];
		for(int i = 0; i < group.GetCount(); i++) {
			tl.group = group.GetKey(i);
			if(all || tl.group == "src" || tl.group == "srcdoc" || tl.group == "srcimp") {
				String n = tl.group;
				if(n == "src")
					n = "Reference";
				if(n == "srcdoc")
					n = "Documents";
				if(n == "srcimp")
					n = "Implementation";
				int gid;
				bool groupfirst = true;
				const Index<String>& topic = group[i];
				for(int i = 0; i < topic.GetCount(); i++) {
					tl.topic = topic[i];
					int q = tl.topic.ReverseFind('$');
					String l;
					if(q >= 0)
						l = ToUpper(tl.topic.Mid(q + 1));
					String link = TopicLinkString(tl);
					if(idelink.Find(link) < 0 && MatchTopicLink(link, sdx) && (lng == "All" || lng == l)) {
						int pd;
						if(used.Find(tl.package) >= 0) {
							if(usedfirst) {
								usid = AddTree(0, IdeImg::Package(), Null, "Used packages");
								usedfirst = false;
							}
							pd = usid;
						}
						else {
							if(otherfirst) {
								if(usedfirst) {
									usid = AddTree(0, IdeImg::Package(), Null, "Used packages");
									usedfirst = false;
								}
								otid = AddTree(0, IdeImg::Package(), Null, "Other packages");
								otherfirst = false;
							}
							pd = otid;
						}
						if(packagefirst) {
							pid = AddTree(pd, TopicImg::Package(), Null, tl.package);
							packagefirst = false;
						}
						if(groupfirst) {
							gid = AddTree(pid, Null, Null, n);
							groupfirst = false;
						}
						String p = TopicLinkString(tl);
						String t = GetTopicTitle(p);
						AddTree(gid, TopicImg::Topic(), p, t);
					}
				}
			}
		}
	}
	if(!idefirst)
		SortTree(ide);
	if(!usedfirst)
		SortTree(usid);
	if(!otherfirst)
		SortTree(otid);
	FinishTree();
	if(sdx.GetCount()) {
		OpenDeep();
		CurrentOrHome();
	}
}

Vector<String> GetTypeRefLinks(const String& t, String &label)
{
	const char *tp[] = { "", "::struct", "::class", "::union", "::typedef", "::enum" };
	Vector<String> f;
	for(int i = 0; i < __countof(tp); i++) {
		label = t + tp[i];
		f = GetRefLinks(label);
		if(f.GetCount())
			break;
	}
	return f;
}

String recent_topic;

Topic TopicCtrl::AcquireTopic(const String& t)
{
	recent_topic = t;
	String topic = t;
	internal = (byte)*topic < 32;
	if(*topic == '\3')
		return GetTopic(topic.Mid(1));
/*	if(*topic == '\1') {
		Topic t;
		t.title = "Summary";
		String group = topic.Mid(1);
		int q = group.Find('\2');
		String package = group.Mid(q + 1);
		group.Trim(q);
		String path = AppendFileName(AppendFileName(PackageDirectory(package), group + ".tpp"),
		                             "index$en-us.tpp");
		if(FileExists(path))
			return ReadTopic(LoadFile(path));
		return Topic();
	}*/
	if(topic[0] == ':' && topic[1] == ':') {
		String lbl;
		Vector<String> link = GetTypeRefLinks(topic, lbl);
		if(link.GetCount() == 0)
			return Topic();
		if(link.GetCount() == 1)
			topic = link[0];
		else {
			WithSimpleListLayout<TopWindow> dlg;
			CtrlLayoutOKCancel(dlg, "Choose one of more link targets");
			dlg.list.AddKey();
			dlg.list.AddColumn("Topic");
			for(int i = 0; i < link.GetCount(); i++)
				dlg.list.Add(link[i], GetTopicTitle(link[i]));
			dlg.list.SetCursor(0);
			if(dlg.Run() != IDOK || !dlg.list.IsCursor())
				return Topic();
			topic = dlg.list.GetKey();
		}
		if(lbl.GetCount())
			topic << '#' << lbl;
	}
	TopicLink tl = ParseTopicLink(topic);
	if(!IsNull(tl.package)) {
		String path = AppendFileName(
						AppendFileName(PackageDirectory(tl.package), tl.group + ".tpp"),
						tl.topic + ".tpp");
		Topic t = ReadTopic(LoadFile(path));
		t.label = tl.label;
		tl.label.Clear();
		t.link = TopicLinkString(tl);
		return t;
	}
	return Topic();
}

struct HighlightWords : RichText::Iterator {
	Index<String> words;
	struct Pos : Moveable<Pos> { int pos, len; };
	Vector<Pos>   pos;

	virtual bool operator()(int tpos, const RichPara& para) {
		WString text = para.GetText();
		const wchar *s = text;
		for(;;) {
			while(!IsLetter(*s) && *s)
				s++;
			if(*s == '\0')
				break;
			WStringBuffer wb;
			const wchar *b = s;
			while(IsLetter(*s))
				wb.Cat(ToLower(*s++));
			if(words.Find(FromUnicode(wb)) >= 0) {
				Pos& p = pos.Add();
				p.pos = int(b - ~text) + tpos;
				p.len = int(s - b);
			}
		}
		return false;
	}
};

void TopicCtrl::FinishText(RichText& text)
{
	if(!showwords)
		return;
	Vector<String> ss = Split((String)~search, ' ');
	if(ss.GetCount() == 0)
		return;
	HighlightWords hw;
	hw.words = ss;
	text.Iterate(hw);
	RichText::FormatInfo fi;
	fi.charvalid = RichText::PAPER|RichText::INK;
	fi.paravalid = 0;
	fi.paper = SColorHighlight();
	fi.ink = SColorHighlightText();
	for(int i = 0; i < hw.pos.GetCount(); i++)
		text.ApplyFormatInfo(hw.pos[i].pos, fi, hw.pos[i].len);
}

void TopicCtrl::OpenTopic()
{
	WhenTopic();
}

struct SlideShow : TopWindow {
	virtual bool Key(dword key, int count);

	RichTextView   text;
	Vector<String> path;
	int            page;
	int            rp;

	void SetPage();

	SlideShow();
};

bool SlideShow::Key(dword key, int count)
{
	switch(key) {
	case K_ESCAPE:
		Break();
		break;
	case K_LEFT:
	case K_UP:
	case K_PAGEUP:
		page--;
		SetPage();
		break;
	case K_RIGHT:
	case K_DOWN:
	case K_PAGEDOWN:
		page++;
		SetPage();
		break;
	}
	return true;
}

void SlideShow::SetPage()
{
	page = minmax(page, 0, path.GetCount() - 1);
	if(page != rp) {
		rp = page;
		text <<= ReadTopic(LoadFile(path[page])).text;
	}
}

SlideShow::SlideShow()
{
	FullScreen();
	Add(text.SizePos());
	text.NoHyperlinkDecoration();
	text.NoSb();
	text.VCenter();
	text.SetZoom(Zoom(1, 5));
	text.Margins(8);
	rp = -1;
}

void TopicCtrl::SShow()
{
	SlideShow ss;
	TopicLink tl = ParseTopicLink(GetCurrent());
	if(IsNull(tl.package))
		return;
	String folder = AppendFileName(PackageDirectory(tl.package), tl.group + ".tpp");
	FindFile ff(AppendFileName(folder, "*.tpp"));
	Array<String> l;
	while(ff) {
		if(ff.IsFile())
			l.Add(GetFileTitle(ff.GetName()));
		ff.Next();
	}
	if(l.GetCount() == 0)
		return;
	Sort(l);
	for(int i = 0; i < l.GetCount(); i++) {
		if(tl.topic == l[i])
			ss.page = i;
		ss.path.Add(AppendFileName(folder, l[i] + ".tpp"));
	}
	ss.SetPage();
	ss.Run();
	tl.topic = l[ss.page];
	GoTo(TopicLinkString(tl));
}

void TopicCtrl::Search()
{
	int l, h;
	search.GetSelection(l, h);
	SyncDocTree();
	SetBar();
	search.SetFocus();
	search.SetSelection(l, h);
}

void TopicCtrl::SearchWord(const String& s)
{
	search <<= s;
	Search();
}

void TopicCtrl::ShowWords()
{
	showwords = !showwords;
	SetBar();
	GoTo(GetCurrent());
}

void TopicCtrl::All()
{
	all = !all;
	SyncDocTree();
	SetBar();
}

void TopicCtrl::Lang()
{
	SyncDocTree();
	SetBar();
}

bool TopicCtrl::Key(dword key, int count)
{
	if(key == K_ENTER && search.HasFocus()) {
		Search();
		return true;
	}
	return HelpWindow::Key(key, count);
}

void TopicCtrl::FocusSearch()
{
	search.SetFocus();
}

void  TopicCtrl::BarEx(Bar& bar)
{
	bar.Gap();
	bar.Add(lang, HorzLayoutZoom(60));
	bar.Add("All topics", IdeImg::HelpAll(), THISBACK(All))
	   .Check(all);
	bar.Gap(HorzLayoutZoom(30));
	bar.Add(search, HorzLayoutZoom(300));
	bar.AddKey(K_CTRL_F, THISBACK(FocusSearch));
/*	bar.Add("Highlight search keywords in topic", IdeImg::ShowWords(), THISBACK(ShowWords))
	   .Check(showwords);*/
	bar.Add(!internal && GetCurrent().StartsWith("topic:"),
	        IdeImg::show(), THISBACK(SShow));
	bar.GapRight();
	bar.Separator();
	bar.Add(!internal && GetCurrent().StartsWith("topic:"),
	        TopicImg::Topic(), THISBACK(OpenTopic));
}

void TopicCtrl::Serialize(Stream& s)
{
	int version = 3;
	s / version;
	if(version < 3) {
		WithDropChoice<EditString> dummy;
		dummy.SerializeList(s);
	}
	if(version >= 1)
		s % showwords;
	if(version >= 2)
		s % all;
}

struct HelpDes : public IdeDesigner {
	TopicCtrl *topic;

	virtual String GetFileName() const              { return HELPNAME; }
	virtual void   Save()                           {}
	virtual void   EditMenu(Bar& menu)              {}
	virtual Ctrl&  DesignerCtrl()                   { return *topic; }
	virtual void   SetFocus()                       { topic->SetFocus(); }
};

bool IsHelpName(const char *path)
{
	return strcmp(path, HELPNAME) == 0;
}

struct HelpModule : public IdeModule {
	virtual Image FileIcon(const char *path) {
		return IsHelpName(path) ? IdeImg::help() : Null;
	}
	virtual IdeDesigner *CreateDesigner(Ide *ide, const char *path, byte cs) {
		if(IsHelpName(path)) {
			topic_serial++;
			GetRefLinks("");
			ide->doc.SyncDocTree();
			ide->doc.GoTo(Nvl(recent_topic, sTopicHome));
			HelpDes *d = new HelpDes;
			d->topic = &ide->doc;
			return d;
		}
		return false;
	}
	virtual void Serialize(Stream& s) {
		s % recent_topic;
	}
};

INITBLOCK {
	RegisterIdeModule(Single<HelpModule>());
}

int CharFilterTopicSearch(int c)
{
	if(c == ' ') return c;
	c = CharFilterDefaultToUpperAscii(c);
	return IsAlNum(c) ? c : 0;
}

TopicCtrl::TopicCtrl()
{
	Icon(IdeImg::doc());
	showwords = true;
	all = false;
	lang <<= THISBACK(Lang);
	lang.Tip("Language"),
	search.NullText("Search", StdFont().Italic(), SColorDisabled());
	search.Tip("Full text search");
	search <<= THISBACK(Search);
	search.SetFilter(CharFilterTopicSearch);
	internal = true;
}

void Ide::ShowTopics()
{
	if(designer && designer->GetFileName() == HELPNAME) {
		CycleFiles();
		tabi = 0;
		return;
	}
	if(doc_serial != TopicEditor::GetSerial()) {
		GetRefLinks("");
		doc.SyncDocTree();
		doc.GoTo(sTopicHome);
		doc_serial = TopicEditor::GetSerial();
	}
	EditFile(HELPNAME);
}

void Ide::SearchTopics()
{
	String s = editor.GetWord();
	GetRefLinks("");
	doc.SyncDocTree();
	doc.GoTo(sTopicHome);
	EditFile(HELPNAME);
	if(s.GetLength())
		doc.SearchWord(s);
}

void Ide::RefreshBrowser()
{
	browser.Refresh();
	doc.SyncDocTree();
}

void Ide::ShowTopic(String link)
{
	ShowTopics();
	doc.GoTo(link + '#' + browser.GetItem());
}
