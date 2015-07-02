#ifndef _ide_Android_Devices_h_
#define _ide_Android_Devices_h_

#include <Core/Core.h>

NAMESPACE_UPP

class AndroidDevice : public Moveable<AndroidDevice> {
public:
	
private:
	String serial;
};

class AndroidVirtualDevice : public Moveable<AndroidVirtualDevice> {
public:
	AndroidVirtualDevice() {}
	virtual ~AndroidVirtualDevice() {}
	
public:
	String GetName() const       { return name; }
	String GetDeviceType() const { return deviceType; }
	String GetPath() const       { return path; }
	String GetTarget() const     { return target; }
	String GetAbi() const        { return abi; }
	Size   GetSize() const       { return size; }
	
	void SetName(const String& name)             { this->name = name; }
	void SetDeviceType(const String& deviceType) { this->deviceType = deviceType; }
	void SetPath(const String& path)             { this->path = path; }
	void SetTarget(const String& target)         { this->target = target; }
	void SetAbi(const String& abi)               { this->abi = abi; }
	void SetSize(const Size& size)               { this->size = size; }
	
private:
	String name;
	String deviceType;
	String path;
	String target;
	String abi;
	Size   size;
};

END_UPP_NAMESPACE

#endif
