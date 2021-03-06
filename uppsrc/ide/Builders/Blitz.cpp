#include "Builders.h"

NAMESPACE_UPP

String BlitzBaseFile()
{
	return ConfigFile("blitzbase");
}

void ResetBlitz()
{
	SaveFile(BlitzBaseFile(), "");
}

Time blitz_base_time;

void InitBlitz()
{
	Time ltm = Time::High();

	const Workspace& wspc = GetIdeWorkspace();
	for(int i = 0; i < wspc.GetCount(); i++) { // find lowest file time
		const Package& pk = wspc.GetPackage(i);
		String n = wspc[i];
		for(int i = 0; i < pk.GetCount(); i++) {
			String path = SourcePath(n, pk.file[i]);
			if(FileExists(path))
				ltm = min(ltm, FileGetTime(path));
		}
	}
	
	blitz_base_time = max(GetSysTime() - 3600, Time(GetFileTime(BlitzBaseFile())));
	if(ltm != Time::High())
		blitz_base_time = max(blitz_base_time, ltm + 3 * 60); // should solve first build after install/checkout
}

Blitz BlitzBuilderComponent::MakeBlitzStep(Vector<String>& sfile, Vector<String>& soptions,
                                           Vector<String>& obj, Vector<String>& immfile,
                                           const char *objext, Vector<bool>& optimize,
                                           const Index<String>& noblitz)
{
	Blitz b;
	b.count = 0;
	b.build = false;

	if(!IsBuilder())
		return b;
	
	Vector<String> excluded;
	Vector<String> excludedoptions;
	Vector<bool>   excludedoptimize;
	b.object = CatAnyPath(builder->outdir, "$blitz" + String(objext));
	Time blitztime = GetFileTime(b.object);
	String blitz;
	if(!IdeGetOneFile().IsEmpty())
		return b;
	for(int i = 0; i < sfile.GetCount(); i++) {
		String fn = sfile[i];
		String ext = ToLower(GetFileExt(fn));
		String objfile = CatAnyPath(builder->outdir, GetFileTitle(fn) + objext);
		Time fntime = GetFileTime(fn);
		if((ext == ".cpp" || ext == ".cc" || ext == ".cxx" || ext == ".icpp")
		   && HdependBlitzApproved(fn) && IsNull(soptions[i]) && !optimize[i]
		   && fntime < blitz_base_time
		   && noblitz.Find(fn) < 0) {
			if(HdependFileTime(fn) > blitztime)
				b.build = true;
			blitz << "\r\n"
			      << "#define BLITZ_INDEX__ F" << i << "\r\n"
			      << "#include \"" << builder->GetHostPath(fn) << "\"\r\n";
			b.info << ' ' << GetFileName(fn);
			const Vector<String>& d = HdependGetDefines(fn);
			for(int i = 0; i < d.GetCount(); i++)
				blitz << "#ifdef " << d[i] << "\r\n"
				      << "#undef " << d[i] << "\r\n"
				      << "#endif\r\n";
			blitz << "#undef BLITZ_INDEX__\r\n";
			b.count++;
		}
		else {
			excluded.Add(fn);
			excludedoptions.Add(soptions[i]);
			excludedoptimize.Add(optimize[i]);
		}
	}
	
	b.path = CatAnyPath(builder->outdir, "$blitz.cpp");
	if(b.count > 1) {
		sfile = pick(excluded);
		soptions = pick(excludedoptions);
		optimize = pick(excludedoptimize);
		if(builder->LoadFile(b.path) != blitz) {
			builder->SaveFile(b.path, blitz);
			b.build = true;
		}
		obj.Add(b.object);
		immfile.Add(b.object);
	}
	else {
		builder->DeleteFile(b.path);
		b.build = false;
	}
	return b;
}

END_UPP_NAMESPACE
