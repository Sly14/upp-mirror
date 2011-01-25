#include "ProtectClient.h"

NAMESPACE_UPP

ProtectClient::ProtectClient()
{
	// generates client id
	clientID = -1;
	
	// defaults to Snow2 Cypher
	cypher = new Snow2;
}

ProtectClient::~ProtectClient()
{
	// if connected, disconnect from server
	Disconnect();
}

// sets the encrypting engine
// default Cypher at startup is Snow2
// WARNING -- takes Cypher ownership
void ProtectClient::SetCypher(Cypher *c)
{
	// sets the new cypher freeing previous one
	cypher = c;
	
}
		
// sends a VectorMap to server in encrypted form
// and gets its response
VectorMap<String, Value> ProtectClient::SendMap(VectorMap<String, Value> const &v)
{
	// copy vectormap adding client id and a magic number
	VectorMap<String, Value>dataMap(v, 1);
	dataMap.Add("APPID", "ProtectClient");
	
	// sets cypher key (and create a random IV)
	cypher->SetKey(key);
	
	String postData;
	postData += "IV=" + HexString(cypher->GetNonce()) + "&";
	postData += "DATA=";
	postData += HexString((*cypher)(StoreAsXML(dataMap, "ProtectClient")));

	client.Post(postData);
	String contents = client.ExecuteRedirect();

	// if contents start with "ERROR", just fetch the error desc
	// and put it in result map
	VectorMap<String, Value> resMap;

	// if contents don't start with IV field signals it
	if(!contents.StartsWith("IV="))
	{
		resMap.Add("ERROR", PROTECT_MISSING_IV);
		resMap.Add("ERRORMSG", ProtectMessage(PROTECT_MISSING_IV));
		return resMap;
	}
	
	// well, we've at least the IV, so we can fetch it and DATA field and decrypt
	StringStream s(contents);
	String line = s.GetLine();
	String IV = ScanHexString(line.Mid(3));
	line = s.GetLine();
	if(!line.StartsWith("DATA="))
	{
		resMap.Add("ERROR", PROTECT_BAD_DATA);
		resMap.Add("ERRORMSG", ProtectMessage(PROTECT_BAD_DATA));
		return resMap;
	}
	
	// decodes DATA field and read VectorMap from it
	cypher->SetKey(key, IV);
	String decoded = (*cypher)(ScanHexString(line.Mid(5)));
	try
	{
		LoadFromXML(resMap, decoded);
	}
	catch(...)
	{
		resMap.Clear();
		resMap.Add("ERROR", PROTECT_BAD_DATA);
		resMap.Add("ERRORMSG", ProtectMessage(PROTECT_BAD_DATA));
	}
	return resMap;
}

// create a persistent link to server
bool ProtectClient::Connect(void)
{
	lastError = 0;
	
	// disconnect first, im case we're already connected
	Disconnect();
	
	// send a connect packet to server
	VectorMap<String, Value>v;
	v.Add("REASON", PROTECT_CONNECT);
	v.Add("EMAIL", userEMail);
	v.Add("CLIENTID", (int)clientID);
	VectorMap<String, Value> res = SendMap(v);
	
	// check for errors
	if(res.Find("ERROR") >= 0)
	{
		lastError = res.Get("ERROR");
		clientID = -1;
		return false;
	}
	
	int i = res.Find("CLIENTID");
	if(i < 0)
		clientID = -1;
	else
		clientID = (int)res[i];
	return true;
}

// disconnect from server
bool ProtectClient::Disconnect(void)
{
	lastError = 0;
	
	// sends a disconnect packet to server
	VectorMap<String, Value>v;
	v.Add("REASON", PROTECT_DISCONNECT);
	v.Add("CLIENTID", (int)clientID);
	VectorMap<String, Value> res = SendMap(v);

	// we're probably disconnected anyways....
	clientID = -1;

	// check for errors
	if(res.Find("ERROR") >= 0)
	{
		lastError = res.Get("ERROR");
		return false;
	}

	return true;
}

// refresh server connection
bool ProtectClient::Refresh(void)
{
	lastError = 0;

	// sends a refresh packet to server
	VectorMap<String, Value>v;
	v.Add("REASON", PROTECT_REFRESH);
	v.Add("CLIENTID", (int)clientID);
	VectorMap<String, Value> res = SendMap(v);

	// check for errors
	if(res.Find("ERROR") >= 0)
	{
		lastError = res.Get("ERROR");
		clientID = -1;
		return false;
	}
	
	return true;
}

// get license key
String ProtectClient::GetKey(void)
{
	lastError = 0;

	// sends a getkey packet to server
	VectorMap<String, Value>v;
	v.Add("REASON", PROTECT_GETKEY);
	v.Add("CLIENTID", (int)clientID);
	VectorMap<String, Value> res = SendMap(v);

	// check for errors
	if(res.Find("ERROR") >= 0)
	{
		lastError = res.Get("ERROR");
		return "";
	}
	if(res.Find("KEY") < 0)
	{
		lastError = 999;
		return "";
	}
	return res.Get("KEY");
}

// gets license info
bool ProtectClient::GetLicenseInfo(void)
{
	lastError = 0;

	// sends a getinfo packet to server
	VectorMap<String, Value>v;
	v.Add("REASON", PROTECT_GETLICENSEINFO);
	v.Add("CLIENTID", (int)clientID);
	VectorMap<String, Value> res = SendMap(v);

	// check for errors
	if(res.Find("ERROR") >= 0)
	{
		lastError = res.Get("ERROR");
		return false;
	}
	if(res.Find("EMAIL") >= 0)			userEMail			= res.Get("EMAIL");
	if(res.Find("NAME") >= 0)			userName			= res.Get("NAME");
	if(res.Find("ADDRESS") >= 0)		userAddress			= res.Get("ADDRESS");
	if(res.Find("COUNTRY") >= 0)		userCountry			= res.Get("COUNTRY");
	if(res.Find("ZIP") >= 0)			userZIP				= res.Get("ZIP");
	if(res.Find("PHONE") >= 0)			userPhone			= res.Get("PHONE");
	if(res.Find("FAX") >= 0)			userFax				= res.Get("FAX");
	if(res.Find("CELL") >= 0)			userCell			= res.Get("CELL");
	if(res.Find("EXPIRETIME") >= 0)		expireTime			= res.Get("EXPIRETIME");
	if(res.Find("NUMLICENSES") >= 0)	numLicenses			= res.Get("NUMLICENSES");
	if(res.Find("ALLOWEDVERSION") >=0)	maxAllowedVersion	= res.Get("ALLOWEDVERSION");

	return true;
}

// updates user data on server
bool ProtectClient::UpdateUserData(void)
{
	lastError = 0;

	// sends a register packet to server
	VectorMap<String, Value>v;
	v.Add("REASON", PROTECT_UPDATEUSERDATA);
	v.Add("CLIENTID", (int)clientID);
	v.Add("EMAIL", userEMail);
	v.Add("NAME", userName);
	v.Add("ADDRESS", userAddress);
	v.Add("COUNTRY", userCountry);
	v.Add("ZIP", userZIP);
	v.Add("PHONE", userPhone);
	v.Add("FAX", userFax);
	v.Add("CELL", userCell);
	VectorMap<String, Value> res = SendMap(v);

	// check for errors
	if(res.Find("ERROR") >= 0)
	{
		lastError = res.Get("ERROR");
		return false;
	}
	return true;
}
		
// register app
bool ProtectClient::Register(void)
{
	lastError = 0;

	// sends a register packet to server
	VectorMap<String, Value>v;
	v.Add("REASON", PROTECT_REGISTER);
	v.Add("EMAIL", userEMail);
	v.Add("NAME", userName);
	v.Add("ADDRESS", userAddress);
	v.Add("COUNTRY", userCountry);
	v.Add("ZIP", userZIP);
	v.Add("PHONE", userPhone);
	v.Add("FAX", userFax);
	v.Add("CELL", userCell);
	VectorMap<String, Value> res = SendMap(v);

	// check for errors
	if(res.Find("ERROR") >= 0)
	{
		lastError = res.Get("ERROR");
		return false;
	}
}

END_UPP_NAMESPACE
