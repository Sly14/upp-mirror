#include "ide.h"

void Ide::SerializeWorkspace(Stream& s) {
	int i;
	int version = 11;
	s / version;
	s.Magic(0x12354);
	if(s.IsStoring()) {
		for(i = 0; i < filedata.GetCount(); i++) {
			String fn = filedata.GetKey(i);
			if(!fn.IsEmpty() && FileExists(fn)) {
				s % fn;
				s % filedata[i].editpos;
				if(version >= 1)
					s % filedata[i].columnline;
			}
		}
		String h;
		s % h;
	}
	else {
		String fn;
		filedata.Clear();
		for(;;) {
			s % fn;
			if(fn.IsEmpty()) break;
			FileData& fd = filedata.GetAdd(fn);
			fd.Clear();
			s % fd.editpos;
			if(version >= 1)
				s % fd.columnline;
		}
	}
	String pk = GetActivePackage();
	s % pk;
	package.FindSetCursor(pk);
	s % tablru;
	s % mainconfigname;
	s % mainconfigparam;
	s % console.verbosebuild;
	s % stoponerrors;
	byte dummy;
	s % dummy;
	s % runarg;
	s % recent_runarg;
	s % rundir;
	s % stdout_file % recent_stdout_file / runmode;
	if(version >= 1)
		s % runexternal;
	s % editfile;
	for(i = 0; i < 10; i++)
		s % bookmark[i];
	editor.Serialize(s);
	if(version >= 5)
		s % editorsplit;
	if(version == 6) {
		String n;
		int v;
		s / v;
		for(int i = 0; i < 10; i++) {
			s % n;
			s / v;
		}
	}
	if(version >= 8) {
		bool dummyb;
		String dummy;
		s % dummyb;
		s % dummy;
	}
	SerializeFindInFiles(s);
	String om;
	s % om;
	s % recentoutput;
	s % recentflags;
	s / editortabsize / indent_amount % indent_spaces;
	for(int j = 0; j < GetIdeModuleCount(); j++)
		GetIdeModule(j).Serialize(s); // -> Workspace
	SerializeWorkspaceConfigs(s);
	SerializeOutputMode(s);
	SerializeClosed(s);
	if(version >= 10) {
		if(tabs_serialize) {
			s % tabs;
		}
	}
	if(version >= 11) {
		s % find_file_search_string;
	}
	if(version >= 12)
		SerializePlacement(s);

}

void Ide::Serialize(Stream& s) {
	int version = 0;
	s.Magic(0x2345432);
	s / version;
	s % last_main;
	s % AnySourceFs();
	s % BasedSourceFs();
	s % AnyPackageFs();
	s % pfsplit;
	s % wesplit;
	package.SerializeSettings(s);
	filelist.SerializeSettings(s);
	s % editorfont;
	s % tfont;
	s % veditorfont;
	s % consolefont;
	s % font1;
	s % font2;
	s % show_status_bar;
	s % toolbar_in_row;
	s % filetabs;
	s % auto_enclose;
	s % show_tabs;
	s % tabs_icons;
	s % tabs_crosses;
	s % tabs_grouping;
	s % tabs_serialize;
	s % tabs_stacking;
	s % force_crlf;
	s % no_parenthesis_indent;
	s % hilite_scope;
	s % hilite_if_endif;
	s % hilite_bracket;
	s % hilite_ifdef;
	s % barline;
	s % qtfsel;
	s % wrap_console_text;
	s % mute_sounds;
	s % line_numbers;
	s % bookmark_pos;
	s % use_target;
	s % default_charset;
	s % header_guards;
	s % insert_include;
	String varsname = GetVarsName();
	s % varsname; // What is the point of this?!
	s % pocfg;
//	if(s.IsLoading())
//		LoadVars(varsname);
	SerializeGlobalConfigs(s);
	doc.Serialize(s);
	s % right_split;
	s % splash_screen;
	s % editor.auto_assist;
	s % editor.commentdp;
	s % bordercolumn;
	s % bordercolor;
	s % hydra1_threads;
	s % gdbSelector;
	if(s.IsLoading())
		console.SetSlots(hydra1_threads);
	s % doc;
	s % chstyle;
	s % astyle_BracketIndent;
	s % astyle_NamespaceIndent;
	s % astyle_BlockIndent;
	s % astyle_CaseIndent;
	s % astyle_ClassIndent;
	s % astyle_LabelIndent;
	s % astyle_SwitchIndent;
	s % astyle_PreprocessorIndent;
	s % astyle_MinInStatementIndentLength;
	s % astyle_MaxInStatementIndentLength;
	s % astyle_BreakClosingHeaderBracketsMode;
	s % astyle_BreakElseIfsMode;
	s % astyle_BreakOneLineBlocksMode;
	s % astyle_SingleStatementsMode;
	s % astyle_BreakBlocksMode;
	s % astyle_BreakClosingHeaderBlocksMode;
	s % astyle_BracketFormatMode;
	s % astyle_ParensPaddingMode;
	s % astyle_ParensUnPaddingMode;
	s % astyle_OperatorPaddingMode;
	s % astyle_EmptyLineFill;
	s % astyle_TabSpaceConversionMode;
	s % astyle_TestBox;
	s % LinuxHostConsole;
	editor.SerializeNavigator(s);
	s % showtime;
	s % DiffFs();
	s % sort;
	s % output_per_assembly;
	s.Magic();
}

void Ide::SaveConfig()
{
	SaveChangedFile(ConfigFile(), StoreAsString(*this));
	SaveChangedFile(ConfigFile("ide.key"), StoreKeys());
	SaveChangedFile(ConfigFile("ide.colors"), editor.StoreHlStyles());
	config_time = FileGetTime(ConfigFile());
}

void Ide::LoadConfig()
{
	LoadFromFile(*this);
	RestoreKeys(LoadFile(ConfigFile("ide.key")));
	editor.LoadHlStyles(LoadFile(ConfigFile("ide.colors")));
	config_time = FileGetTime(ConfigFile());
	UpdateFormat();
	if(filelist.IsCursor()) {
		FlushFile();
		FileCursor();
	}
	SaveLoadPackage();
	SyncCh();
}

void Ide::CheckConfig()
{
	if(config_time != FileGetTime(ConfigFile()))
		LoadConfig();
}
