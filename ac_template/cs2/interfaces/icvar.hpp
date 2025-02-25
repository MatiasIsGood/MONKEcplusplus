#pragma once
#include <cstdint>
#include <ranges>
#include <string>

//-----------------------------------------------------------------------------
// ConVar flags
//-----------------------------------------------------------------------------
// The default, no flags at all
#define FCVAR_NONE				0 

// Command to ConVars and ConCommands
// ConVar Systems
#define FCVAR_LINKED_CONCOMMAND (1<<0)
#define FCVAR_DEVELOPMENTONLY	(1<<1)	// Hidden in released products. Flag is removed automatically if ALLOW_DEVELOPMENT_CVARS is defined.
#define FCVAR_GAMEDLL			(1<<2)	// defined by the game DLL
#define FCVAR_CLIENTDLL			(1<<3)  // defined by the client DLL
#define FCVAR_HIDDEN			(1<<4)	// Hidden. Doesn't appear in find or auto complete. Like DEVELOPMENTONLY, but can't be compiled out.

// ConVar only
#define FCVAR_PROTECTED			(1<<5)  // It's a server cvar, but we don't send the data since it's a password, etc.  Sends 1 if it's not bland/zero, 0 otherwise as value
#define FCVAR_SPONLY			(1<<6)  // This cvar cannot be changed by clients connected to a multiplayer server.
#define	FCVAR_ARCHIVE			(1<<7)	// set to cause it to be saved to vars.rc
#define	FCVAR_NOTIFY			(1<<8)	// notifies players when changed
#define	FCVAR_USERINFO			(1<<9)	// changes the client's info string

#define FCVAR_MISSING0	 		(1<<10) // Something that hides the cvar from the cvar lookups
#define FCVAR_UNLOGGED			(1<<11)  // If this is a FCVAR_SERVER, don't log changes to the log file / console if we are creating a log
#define FCVAR_MISSING1			(1<<12)  // Something that hides the cvar from the cvar lookups

// It's a ConVar that's shared between the client and the server.
// At signon, the values of all such ConVars are sent from the server to the client (skipped for local
//  client, of course )
// If a change is requested it must come from the console (i.e., no remote client changes)
// If a value is changed while a server is active, it's replicated to all connected clients
#define FCVAR_REPLICATED		(1<<13)	// server setting enforced on clients, TODO rename to FCAR_SERVER at some time
#define FCVAR_CHEAT				(1<<14) // Only useable in singleplayer / debug / multiplayer & sv_cheats
#define FCVAR_PER_USER			(1<<15) // causes varnameN where N == 2 through max splitscreen slots for mod to be autogenerated
#define FCVAR_DEMO				(1<<16) // record this cvar when starting a demo file
#define FCVAR_DONTRECORD		(1<<17) // don't record these command in demofiles
#define FCVAR_MISSING2			(1<<18)
#define FCVAR_RELEASE			(1<<19) // Cvars tagged with this are the only cvars avaliable to customers
#define FCVAR_MENUBAR_ITEM		(1<<20)
#define FCVAR_MISSING3			(1<<21)

#define FCVAR_NOT_CONNECTED		(1<<22)	// cvar cannot be changed by a client that is connected to a server
#define FCVAR_VCONSOLE_FUZZY_MATCHING (1<<23)

#define FCVAR_SERVER_CAN_EXECUTE	(1<<24) // the server is allowed to execute this command on clients via ClientCommand/NET_StringCmd/CBaseClientState::ProcessStringCmd.
#define FCVAR_CLIENT_CAN_EXECUTE	(1<<25) // Assigned to commands to let clients execute them
#define FCVAR_SERVER_CANNOT_QUERY	(1<<26) // If this is set, then the server is not allowed to query this cvar's value (via IServerPluginHelpers::StartQueryCvarValue).
#define FCVAR_VCONSOLE_SET_FOCUS	(1<<27)
#define FCVAR_CLIENTCMD_CAN_EXECUTE	(1<<28)	// IVEngineClient::ClientCmd is allowed to execute this command. 
											// Note: IVEngineClient::ClientCmd_Unrestricted can run any client command.

#define FCVAR_EXECUTE_PER_TICK		(1<<29)

enum EConVarType : std::int32_t
{
	EConVarType_Invalid = -1,
	EConVarType_Bool,
	EConVarType_Int16,
	EConVarType_UInt16,
	EConVarType_Int32,
	EConVarType_UInt32,
	EConVarType_Int64,
	EConVarType_UInt64,
	EConVarType_Float32,
	EConVarType_Float64,
	EConVarType_String,
	EConVarType_Color,
	EConVarType_Vector2,
	EConVarType_Vector3,
	EConVarType_Vector4,
	EConVarType_Qangle
};

using Color = std::uint8_t[4];
using Vector2D = float[2];
using Vector4D = float[4];
using Vector = float[3];
using QAngle = float[3];

union CVValue_t
{
	bool		  m_bValue;
	short		  m_i16Value;
	std::uint16_t m_u16Value;
	int			  m_i32Value;
	std::uint32_t m_u32Value;
	std::int64_t  m_i64Value;
	std::uint64_t m_u64Value;
	float		  m_flValue;
	double		  m_dbValue;
	const char* m_szValue;
	Color		  m_clrValue;
	Vector2D	  m_vec2Value;
	Vector		  m_vec3Value;
	Vector4D	  m_vec4Value;
	QAngle		  m_angValue;
};

class CConVar
{
public:
	const char* m_sName;
	CVValue_t* m_pDefaultValue;
	CVValue_t* m_pMinValue;
	CVValue_t* m_pMaxValue;
	const char* m_sDescription;
	EConVarType m_eType;

	std::uint32_t m_uTimesChanged;
	std::int64_t m_iFlags;
	std::uint32_t m_uCallbackIindex;
	std::int32_t m_iOtherFlags;

	CVValue_t m_oValue;
};

struct ConVarElem
{
	CConVar* element;
	std::uint16_t iPrevious;
	std::uint16_t iNext;
};

struct CVarList
{
	ConVarElem* pMemory;
	int nAllocationCount;
	int nGrowSize;
	std::uint16_t iHead;
	std::uint16_t iTail;
	std::uint16_t iFirstFree;
	std::uint16_t nElementCount;
	std::uint16_t nAllocated;
	void* pElements;
};

struct ICvar
{
	char pad01[0x40];
	CVarList m_oConVars;

	[[nodiscard]] static ConVarElem* GetAllCvars();
	[[nodiscard]] inline CConVar* FindVar(const std::string& name) const noexcept;
	[[nodiscard]] static CConVar* Find(const std::string& name);
	[[nodiscard]] static std::uint16_t GetCount();
	[[nodiscard]] static std::uint16_t IndexOf(const std::string& name);

private:
	[[nodiscard]] static ICvar* GetInterface();
};