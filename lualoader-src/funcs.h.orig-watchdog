
#include<fstream>
#include <windows.h>
#include <winuser.h>
#include<thread> // std::thread
#include<chrono> // std::thread
#include <process.h>

#include <sstream>
#include <cstdio>
#include <cstdlib>

#include <filesystem>
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <map>
#include <mutex>

#include <algorithm>
#include<lua.hpp>

#include "common.h"
#include "plugin.h"
#include "CWorld.h"
#include "extensions/KeyCheck.h"
#include "extensions/ScriptCommands.h"
#include "eScriptCommands.h"
#include "CCivilianPed.h"
#include "CMessages.h"
#include "ePedType.h"
#include "CModelInfo.h"

#include "CStreaming.h"
#include "CTheScripts.h"
#include "eWeaponType.h"
#include "CPickups.h"
#include "CClock.h"
#include "CUserDisplay.h"
#include "CAnimManager.cpp"
#include "CBaseModelInfo.cpp"
#include "CCamera.h"
#include "CWeapon.h"
#include "CPlayerPed.h"
#include "CWeaponInfo.h"
#include "CShotInfo.h"
#include "CMenuManager.h"
#include "CGame.h"// gGameState для plugin.cpp (замена хардкода 0x9B5F08).
#include "CTimer.h"// CTimer::m_snTimeInMilliseconds для plugin.cpp (замена хардкода 0x974B2C).
#include "tHandlingData.h"
#include "CMatrix.h"
#include "CVector.h"
#include "CCopPed.h"
#include "CHeli.h"
#include "CWeather.h"
#include "CProjectileInfo.h"
#include "CBulletTraces.h"
#include "C3dMarkers.h"
#include "CAutomobile.h"
#include "CHud.h"
#include "CPlayerInfo.h"
#include "CBoat.h"
#include "CBike.h"
#include "CTrafficLights.h"
#include "CCutsceneMgr.h"
#include "CCarCtrl.h"
#include "CBrightLights.h"
#include "CTrain.h"
#include "CGangs.h"
#include "CMotionBlurStreaks.h"
#include "CCoronas.h"
#include "CDraw.h"
#include "CWanted.h"
#include "CFire.h"
#include "CFireManager.h"
#include "CStats.h"
#include "CRadar.h"
#include "CPopulation.h"
#include "CGeneral.h"
#include "CGangs.h"
#include "CGameLogic.h"
#include "CFont.h"
#include "eLevelName.h"
#include "CZone.h"
#include "CTheZones.h"
#include "CEscalators.h"
#include "CCranes.h"


using namespace plugin;
using namespace std;

static unsigned int iters = 0;
mutex mtx;
list<char*>listfile;//Список Lua файлов.

list<lua_State*>luastate;// list для lua состояний.
static map<int, lua_State*>markeron;// map для маркеров.
static map<int, lua_State*>spheres;// map для маркеров кругов.
static map<int, lua_State*>pickupsids;// map для пикапов.
static map<int, lua_State*>firesids;// map для огня.
static map<int, lua_State*>soundsids;// map для непрерывных звуков.

static map<CPed*, lua_State*>mappeds;// map для педов.
static map<CVehicle*, lua_State*>mapcars;// map для авто.
static map<CObject*, lua_State*>mapobjs;// map для объектов.

static map<int, CPed*>ptr_for_ped;//
static string cheatstr;// символы введенные с клавиатуры.
string getkey(int key);


map<string, int> car_model_list = { {"MODEL_LANDSTAL", 130}, {"MODEL_IDAHO", 131}, {"MODEL_STINGER", 132}, {"MODEL_LINERUN", 133},
	{"MODEL_PEREN", 134}, {"MODEL_SENTINEL", 135}, {"MODEL_RIO", 136}, {"MODEL_FIRETRUK", 137}, {"MODEL_TRASH", 138},
	{"MODEL_STRETCH", 139}, {"MODEL_MANANA", 140}, {"MODEL_INFERNUS", 141}, {"MODEL_VOODOO", 142}, {"MODEL_PONY", 143},
	{"MODEL_MULE", 144}, {"MODEL_CHEETAH", 145}, {"MODEL_AMBULAN", 146}, {"MODEL_FBICAR", 147}, {"MODEL_MOONBEAM", 148},
	{"MODEL_ESPERANT", 149}, {"MODEL_TAXI", 150}, {"MODEL_WASHING", 151}, {"MODEL_BOBCAT", 152}, {"MODEL_MRWHOOP", 153},
	{"MODEL_BFINJECT", 154}, {"MODEL_HUNTER", 155}, {"MODEL_POLICE", 156}, {"MODEL_ENFORCER", 157}, {"MODEL_SECURICA", 158},
	{"MODEL_BANSHEE", 159}, {"MODEL_PREDATOR", 160}, {"MODEL_BUS", 161}, {"MODEL_RHINO", 162}, {"MODEL_BARRACKS", 163},
	{"MODEL_CUBAN", 164}, {"MODEL_CHOPPER", 165}, {"MODEL_ANGEL", 166}, {"MODEL_COACH", 167}, {"MODEL_CABBIE", 168},
	{"MODEL_STALLION", 169}, {"MODEL_RUMPO", 170}, {"MODEL_RCBANDIT", 171}, {"MODEL_ROMERO", 172}, {"MODEL_PACKER", 173},
	{"MODEL_SENTXS", 174}, {"MODEL_ADMIRAL", 175}, {"MODEL_SQUALO", 176}, {"MODEL_SEASPAR", 177}, {"MODEL_PIZZABOY", 178},
	{"MODEL_GANGBUR", 179}, {"MODEL_AIRTRAIN", 180}, {"MODEL_DEADDODO", 181}, {"MODEL_SPEEDER", 182}, {"MODEL_REEFER", 183},
	{"MODEL_TROPIC", 184}, {"MODEL_FLATBED", 185}, {"MODEL_YANKEE", 186}, {"MODEL_CADDY", 187}, {"MODEL_ZEBRA", 188},
	{"MODEL_TOPFUN", 189}, {"MODEL_SKIMMER", 190}, {"MODEL_PCJ600", 191}, {"MODEL_FAGGIO", 192}, {"MODEL_FREEWAY", 193},
	{"MODEL_RCBARON", 194}, {"MODEL_RCRAIDER", 195}, {"MODEL_GLENDALE", 196}, {"MODEL_OCEANIC", 197}, {"MODEL_SANCHEZ", 198},
	{"MODEL_SPARROW", 199}, {"MODEL_PATRIOT", 200}, {"MODEL_LOVEFIST", 201}, {"MODEL_COASTG", 202}, {"MODEL_DINGHY", 203},
	{"MODEL_HERMES", 204}, {"MODEL_SABRE", 205}, {"MODEL_SABRETUR", 206}, {"MODEL_PHEONIX", 207}, {"MODEL_WALTON", 208},
	{"MODEL_REGINA", 209}, {"MODEL_COMET", 210}, {"MODEL_DELUXO", 211}, {"MODEL_BURRITO", 212}, {"MODEL_SPAND", 213},
	{"MODEL_MARQUIS", 214}, {"MODEL_BAGGAGE", 215}, {"MODEL_KAUFMAN", 216}, {"MODEL_MAVERICK", 217}, {"MODEL_VCNMAV", 218},
	{"MODEL_RANCHER", 219}, {"MODEL_FBIRANCH", 220}, {"MODEL_VIRGO", 221}, {"MODEL_GREENWOO", 222}, {"MODEL_JETMAX", 223},
	{"MODEL_HOTRING", 224}, {"MODEL_SANDKING", 225}, {"MODEL_BLISTAC", 226}, {"MODEL_POLMAV", 227}, {"MODEL_BOXVILLE", 228},
	{"MODEL_BENSON", 229}, {"MODEL_MESA", 230}, {"MODEL_RCGOBLIN", 231}, {"MODEL_HOTRINA", 232}, {"MODEL_HOTRINB", 233},
	{"MODEL_BLOODRA", 234}, {"MODEL_BLOODRB", 235}, {"MODEL_VICECHEE", 236} };

map<string, int> name_weapon_list = { {"unarmed", 0}, {"brassknuckle", 259}, {"screwdriver", 260}, {"golfclub", 261},
{"nitestick", 262}, {"knifecur", 263}, {"bat", 264}, {"hammer", 265}, {"cleaver", 266}, {"machete", 267}, {"katana", 268},
{"chnsaw", 269}, {"grenade", 270}, {"teargas", 271}, {"molotov", 272}, {"missile", 273}, {"colt45", 274}, {"python", 275},
{"ruger", 276}, {"chromegun", 277}, {"shotgspa", 278}, {"buddyshot", 279}, {"m4", 280}, {"tec9", 281}, {"uzi", 282},
{"ingramsl", 283}, {"mp5lng", 284}, {"sniper", 285}, {"laser", 286}, {"rocketla", 287}, {"flame", 288}, {"M60", 289},
{"minigun", 290}, {"bomb", 291}, {"camera", 292}, {"fingers", 293}, {"minigun2", 294} };
map<string, int> types_weapon_list = { {"unarmed", 0}, {"brassknuckle", 1}, {"screwdriver", 2}, {"golfclub", 3}, {"nitestick", 4},
{"knifecur", 5}, {"bat", 6}, {"hammer", 7}, {"cleaver", 8}, {"machete", 9}, {"katana", 10}, {"chnsaw", 11}, {"grenade", 12},
{"bomb", 13}, {"teargas", 14}, {"molotov", 15}, {"missile", 16}, {"colt45", 17}, {"python", 18}, {"chromegun", 19}, {"shotgspa", 20},
{"buddyshot", 21}, {"tec9", 22}, {"uzi", 23}, {"ingramsl", 24}, {"mp5lng", 25}, {"m4", 26}, {"ruger", 27},  {"sniper", 28},
{"laser", 29}, {"rocketla", 30}, {"flame", 31}, {"M60", 32}, {"M60", 32}, {"minigun", 33}, {"fingers", 34},
{"minigun2", 35}, {"camera", 36} };

map<string, int> name_peds_list = { {"ARMY", 4}, {"BFOBE", 40}, {"BFORI", 36}, {"BFOST", 32}, {"BFOTR", 44}, {"BFYBE", 38},
{"BFYPR", 43}, {"BFYRI", 35}, {"BFYST", 31}, {"BKA", 93}, {"BKB", 94}, {"BMOBE", 41}, {"BMODK", 29}, {"BMOST", 34}, {"BMOTR", 45},
{"BMYBB", 47}, {"BMYBE", 39}, {"BMYBU", 42}, {"BMYCR", 30}, {"BMYPI", 46}, {"BMYRI", 37}, {"BMYST", 33}, {"CBA", 83}, {"CBB", 84},
{"CLA", 89}, {"CLB", 90}, {"COP", 1}, {"FBI", 3}, {"FIREMAN", 6}, {"GDA", 91}, {"GDB", 92}, {"HFOBE", 18}, {"HFORI", 14},
{"HFOST", 10}, {"HFOTR", 25}, {"HFYBE", 17}, {"HFYBU", 21}, {"HFYCG", 23}, {"HFYMD", 22}, {"HFYPR", 24}, {"HFYRI", 13}, {"HFYST", 9},
{"HMOBE", 20}, {"HMOCA", 28}, {"HMORI", 16}, {"HMOST", 12}, {"HMOTR", 26}, {"HMYAP", 27}, {"HMYBE", 19}, {"HMYRI", 15}, {"HMYST", 11},
{"HNA", 85}, {"HNB", 86}, {"JFOTO", 81}, {"JMOTO", 82}, {"MEDIC", 5}, {"PGA", 95}, {"PGB", 96}, {"SGA", 87}, {"SGB", 88}, {"SWAT", 2},
{"VICE1", 97}, {"VICE2", 98}, {"VICE3", 99}, {"VICE4", 100}, {"VICE5", 101}, {"VICE6", 102}, {"VICE7", 103}, {"VICE8", 104},
{"WFOBE", 59}, {"WFOGO", 63}, {"WFORI", 54}, {"WFOSH", 80}, {"WFOST", 50}, {"WFOTR", 71}, {"WFYBE", 57}, {"WFYBU", 67}, {"WFYG1", 105},
{"WFYG2", 106}, {"WFYJG", 75}, {"WFYLG", 65}, {"WFYPR", 70}, {"WFYRI", 53}, {"WFYSH", 79}, {"WFYSK", 77}, {"WFYST", 49}, {"WMOBE", 60},
{"WMOBU", 69}, {"WMOCA", 74}, {"WMOGO", 64}, {"WMORI", 56}, {"WMOST", 52}, {"WMOTR", 72}, {"WMYBE", 58}, {"WMYBU", 68}, {"WMYCR", 48},
{"WMYCW", 61}, {"WMYGO", 62}, {"WMYJG", 76}, {"WMYLG", 66}, {"WMYPI", 73}, {"WMYRI", 55}, {"WMYSK", 78}, {"WMYST", 51} };

map<string, int> type_peds_list = { {"ARMY", 6}, {"BFOBE", 5}, {"BFORI", 5}, {"BFOST", 5}, {"BFOTR", 5}, {"BFYBE", 5}, {"BFYPR", 5},
{"BFYRI", 5}, {"BFYST", 5}, {"BKA", 12}, {"BKB", 12}, {"BMOBE", 4}, {"BMODK", 4}, {"BMOST", 4}, {"BMOTR", 4}, {"BMYBB", 4},
{"BMYBE", 4}, {"BMYBU", 4}, {"BMYCR", 4}, {"BMYPI", 4}, {"BMYRI", 4}, {"BMYST", 4}, {"CBA", 7}, {"CBB", 7}, {"CLA", 10}, {"CLB", 10},
{"COP", 6}, {"FBI", 6}, {"FIREMAN", 17}, {"GDA", 11}, {"GDB", 11}, {"HFOBE", 5}, {"HFORI", 5}, {"HFOST", 5}, {"HFOTR", 5}, {"HFYBE", 5},
{"HFYBU", 5}, {"HFYCG", 5}, {"HFYMD", 5}, {"HFYPR", 5}, {"HFYRI", 5}, {"HFYST", 5}, {"HMOBE", 4}, {"HMOCA", 4}, {"HMORI", 4},
{"HMOST", 4}, {"HMOTR", 4}, {"HMYAP", 4}, {"HMYBE", 4}, {"HMYRI", 4}, {"HMYST", 4}, {"HNA", 8}, {"HNB", 8}, {"JFOTO", 5}, {"JMOTO", 4},
{"MEDIC", 16}, {"PGA", 13}, {"PGB", 13}, {"SGA", 9}, {"SGB", 9}, {"SWAT", 6}, {"VICE1", 18}, {"VICE2", 18}, {"VICE3", 18},
{"VICE4", 18}, {"VICE5", 18}, {"VICE6", 18}, {"VICE7", 18}, {"VICE8", 18}, {"WFOBE", 5}, {"WFOGO", 5}, {"WFORI", 5}, {"WFOSH", 5},
{"WFOST", 5}, {"WFOTR", 5}, {"WFYBE", 5}, {"WFYBU", 5}, {"WFYG1", 5}, {"WFYG2", 6}, {"WFYJG", 5}, {"WFYLG", 5}, {"WFYPR", 5},
{"WFYRI", 5}, {"WFYSH", 5}, {"WFYSK", 5}, {"WFYST", 5}, {"WMOBE", 4}, {"WMOBU", 4}, {"WMOCA", 4}, {"WMOGO", 4}, {"WMORI", 4},
{"WMOST", 4}, {"WMOTR", 4}, {"WMYBE", 4}, {"WMYBU", 4}, {"WMYCR", 4}, {"WMYCW", 4}, {"WMYGO", 4}, {"WMYJG", 4}, {"WMYLG", 4},
{"WMYPI", 4}, {"WMYRI", 4}, {"WMYSK", 4}, {"WMYST", 4} };

map<string, int> type_specpeds_list = { {"BGA", 4}, {"BGB", 4}, {"BOUNCA", 4}, {"BURGER", 4}, {"CGONA", 4}, {"CGONB", 4}, {"CGONC", 4},
{"CHEF", 4}, {"CMRAMAN", 4}, {"COURIER", 4}, {"CREWA", 4}, {"CREWB", 4}, {"CSJUGGZ", 5}, {"DGOONA", 4}, {"DGOONB", 4}, {"DGOONC", 4},
{"FLOOZYA", 5}, {"FLOOZYB", 5}, {"FLOOZYC", 5}, {"FSFA", 4}, {"IGALSCB", 4}, {"IGBUDDY", 4}, {"IGBUDY2", 4}, {"IGBUDY3", 4},
{"IGCANDY", 5}, {"IGCOLON", 4}, {"IGDIAZ", 4}, {"IGDICK", 4}, {"IGGONZ", 4}, {"IGHLARY", 4}, {"IGHLRY2", 4}, {"IGJEZZ", 4},
{"IGKEN", 4}, {"IGMERC", 5}, {"IGMERC2", 5}, {"IGMIKE", 4}, {"IGMIKE2", 4}, {"IGPERCY", 4}, {"IGPHIL", 4}, {"IGPHIL2", 4},
{"IGPHIL3", 4}, {"IGSONNY", 4}, {"MBA", 4}, {"MBB", 4}, {"MGOONA", 4}, {"MPORNA", 4}, {"MSERVER", 4}, {"PLAY10", 4}, {"PLAY11", 4},
{"PLAY12", 4}, {"PLAYER2", 4}, {"PLAYER3", 4}, {"PLAYER4", 4}, {"PLAYER5", 4}, {"PLAYER6", 4}, {"PLAYER7", 4}, {"PLAYER8", 4},
{"PLAYER9", 4}, {"PRINTRA", 4}, {"PRINTRB", 4}, {"PRINTRC", 4}, {"PSYCHO", 4}, {"SAM", 4}, {"SGC", 4}, {"SGOONA", 4}, {"SGOONB", 4},
{"SHOOTRA", 5}, {"SHOOTRB", 4}, {"SPANDXA", 5}, {"SPANDXB", 5}, {"STRIPA", 5}, {"S_KEEP", 4} };

map<int, int> type_and_model = { {0, 0}, {1, 259}, {2, 260}, {3, 261}, {4, 262}, {5, 263}, {6, 264}, {7, 265},
{8, 266}, {9, 267}, {10, 268}, {11, 269}, {12, 270}, {13, 291}, {14, 271}, {15, 272}, {16, 273}, {17, 274},
{18, 275}, {19, 277}, {20, 278}, {21, 279}, {22, 281}, {23, 282}, {24, 283}, {25, 284}, {26, 280}, {27, 276},
{29, 286}, {30, 287}, {31, 288}, {32, 289}, {32, 285}, {32, 289}, {32, 285}, {33, 290}, {34, 293}, {35, 294},
{36, 292} };

map<int, int> model_and_type = { {0, 0}, {259, 1}, {260, 2}, {261, 3}, {262, 4}, {263, 5}, {264, 6}, {265, 7},
{266, 8}, {267, 9}, {268, 10}, {269, 11}, {270, 12}, {271, 14}, {272, 15}, {273, 16}, {274, 17}, {275, 18}, {276, 27},
{277, 19}, {278, 20}, {279, 21}, {280, 26}, {281, 22}, {282, 23}, {283, 24}, {284, 25}, {285, 32}, {286, 29}, {287, 30},
{288, 31}, {289, 32}, {290, 33}, {291, 13}, {292, 36}, {293, 34}, {294, 35} };

map<string, int> numbers_keys = { {"VK_A", 65}, {"VK_ABNT_C1", 193}, {"VK_ABNT_C2", 194}, {"VK_ACCEPT", 30},
{"VK_ADD", 107}, {"VK_APPS", 93}, {"VK_ATTN", 246}, {"VK_B", 66}, {"VK_BACK", 8}, {"VK_BROWSER_BACK", 166},
{"VK_BROWSER_FAVORITES", 171}, {"VK_BROWSER_FORWARD", 167}, {"VK_BROWSER_HOME", 172}, {"VK_BROWSER_REFRESH", 168},
{"VK_BROWSER_SEARCH", 170}, {"VK_BROWSER_STOP", 169}, {"VK_C", 67}, {"VK_CANCEL", 3}, {"VK_CAPITAL", 20}, {"VK_CLEAR", 12},
{"VK_CONTROL", 17}, {"VK_CONVERT", 28}, {"VK_CRSEL", 247}, {"VK_D", 68}, {"VK_DECIMAL", 110}, {"VK_DELETE", 46},
{"VK_DIVIDE", 111}, {"VK_DOWN", 40}, {"VK_E", 69}, {"VK_END", 35}, {"VK_EREOF", 249}, {"VK_ESCAPE", 27}, {"VK_EXECUTE", 43},
{"VK_EXSEL", 248}, {"VK_F", 70}, {"VK_F1", 112}, {"VK_F10", 121}, {"VK_F11", 122}, {"VK_F12", 123}, {"VK_F13", 124},
{"VK_F14", 125}, {"VK_F15", 126}, {"VK_F16", 127}, {"VK_F17", 128}, {"VK_F18", 129}, {"VK_F19", 130}, {"VK_F2", 113},
{"VK_F20", 131}, {"VK_F21", 132}, {"VK_F22", 133}, {"VK_F23", 134}, {"VK_F24", 135}, {"VK_F3", 114}, {"VK_F4", 115},
{"VK_F5", 116}, {"VK_F6", 117}, {"VK_F7", 118}, {"VK_F8", 119}, {"VK_F9", 120}, {"VK_FINAL", 24}, {"VK_G", 71},
{"VK_H", 72}, {"VK_HELP", 47}, {"VK_HOME", 36}, {"VK_I", 73}, {"VK_ICO_CLEAR", 230}, {"VK_ICO_HELP", 227},
{"VK_INSERT", 45}, {"VK_J", 74}, {"VK_JUNJA", 23}, {"VK_K", 75}, {"VK_KANA", 21}, {"VK_KANJI", 25}, {"VK_L", 76},
{"VK_LAUNCH_APP1", 182}, {"VK_LAUNCH_APP2", 183}, {"VK_LAUNCH_MAIL", 180}, {"VK_LAUNCH_MEDIA_SELECT", 181},
{"VK_LBUTTON", 1}, {"VK_LCONTROL", 162}, {"VK_LEFT", 37}, {"VK_LMENU", 164}, {"VK_LSHIFT", 160}, {"VK_LWIN", 91},
{"VK_M", 77}, {"VK_MBUTTON", 4}, {"VK_MEDIA_NEXT_TRACK", 176}, {"VK_MEDIA_PLAY_PAUSE", 179}, {"VK_MEDIA_PREV_TRACK", 177},
{"VK_MEDIA_STOP", 178}, {"VK_MENU", 18}, {"VK_MODECHANGE", 31}, {"VK_MULTIPLY", 106}, {"VK_N", 78}, {"VK_NEXT", 34},
{"VK_NONCONVERT", 29}, {"VK_NUMLOCK", 144}, {"VK_NUMPAD0", 96}, {"VK_NUMPAD1", 97}, {"VK_NUMPAD2", 98}, {"VK_NUMPAD3", 99},
{"VK_NUMPAD4", 100}, {"VK_NUMPAD5", 101}, {"VK_NUMPAD6", 102}, {"VK_NUMPAD7", 103}, {"VK_NUMPAD8", 104}, {"VK_NUMPAD9", 105},
{"VK_O", 79}, {"VK_OEM_1", 186}, {"VK_OEM_102", 226}, {"VK_OEM_2", 191}, {"VK_OEM_3", 192}, {"VK_OEM_4", 219},
{"VK_OEM_5", 220}, {"VK_OEM_6", 221}, {"VK_OEM_7", 222}, {"VK_OEM_8", 223}, {"VK_OEM_ATTN", 240}, {"VK_OEM_AUTO", 243},
{"VK_OEM_AX", 225}, {"VK_OEM_BACKTAB", 245}, {"VK_OEM_CLEAR", 254}, {"VK_OEM_COMMA", 188}, {"VK_OEM_COPY", 242},
{"VK_OEM_CUSEL", 239}, {"VK_OEM_ENLW", 244}, {"VK_OEM_FINISH", 241}, {"VK_OEM_FJ_JISHO", 146}, {"VK_OEM_FJ_LOYA", 149},
{"VK_OEM_FJ_MASSHOU", 147}, {"VK_OEM_FJ_ROYA", 150}, {"VK_OEM_FJ_TOUROKU", 148}, {"VK_OEM_JUMP", 234}, {"VK_OEM_MINUS", 189},
{"VK_OEM_PA1", 235}, {"VK_OEM_PA2", 236}, {"VK_OEM_PA3", 237}, {"VK_OEM_PERIOD", 190}, {"VK_OEM_PLUS", 187},
{"VK_OEM_RESET", 233}, {"VK_OEM_WSCTRL", 238}, {"VK_P", 80}, {"VK_PA1", 253}, {"VK_PACKET", 231}, {"VK_PAUSE", 19},
{"VK_PLAY", 250}, {"VK_PRINT", 42}, {"VK_PRIOR", 33}, {"VK_PROCESSKEY", 229}, {"VK_Q", 81}, {"VK_R", 82},
{"VK_RBUTTON", 2}, {"VK_RCONTROL", 163}, {"VK_RETURN", 13}, {"VK_RIGHT", 39}, {"VK_RMENU", 165}, {"VK_RSHIFT", 161},
{"VK_RWIN", 92}, {"VK_S", 83}, {"VK_SCROLL", 145}, {"VK_SELECT", 41}, {"VK_SEPARATOR", 108}, {"VK_SHIFT", 16},
{"VK_SLEEP", 95}, {"VK_SNAPSHOT", 44}, {"VK_SPACE", 32}, {"VK_SUBTRACT", 109}, {"VK_T", 84}, {"VK_TAB", 9},
{"VK_U", 85}, {"VK_UP", 38}, {"VK_V", 86}, {"VK_VOLUME_DOWN", 174}, {"VK_VOLUME_MUTE", 173}, {"VK_VOLUME_UP", 175},
{"VK_W", 87}, {"VK_X", 88}, {"VK_XBUTTON1", 5}, {"VK_XBUTTON2", 6}, {"VK_Y", 89}, {"VK_Z", 90}, {"VK_ZOOM", 251},
{"VK_key0", 48}, {"VK_key1", 49}, {"VK_key2", 50}, {"VK_key3", 51}, {"VK_key4", 52}, {"VK_key5", 53}, {"VK_key6", 54},
{"VK_key7", 55}, {"VK_key8", 56}, {"VK_key9", 57} };

int find_in_map(std::map<string, int>& carlist, const char* search);// найти id авто.
int find_model_in_map(std::map<int, int>& type_and_model, int search);// найти модель авто.

int& var_$3402 = *(int*)0x8247A8; // глобальнная переменная таймера.

CPed* findpedinpool(const void* p); // найти педа в пуле.
CVehicle* findcarinpool(const void* p); // найти авто в пуле.
CObject* findobjinpool(const void* p);// найти объект в пуле.
void warp_out_car_occupants(CVehicle* car);// выкинуть всех сидящих в авто.
void safe_remove_car(CVehicle* car);// безопасно удалить авто.
void warp_ped_out_of_car(CPed* ped);// выкинуть педа из авто, если сидит.

void getkeyenvent(); // считывания символов клавиатуры.
wchar_t* getwchat(const char* c); // перевод в строку.

int funs(lua_State* L);// список функций.
void writelog(const char x[]);// запись ошибок в файл.
void cpp_trace(const char x[]);// запись трейса C++ (в тот же trace.txt).
void cpp_tracef(const char* fmt, ...);// форматированная запись трейса C++.
void dellod(); // удалить лог ошибок.
int cleanstl();// очистка stl.

int wait(lua_State* L);// задержка
int findplayer(lua_State* L);// найти педа.
int getpedhealth(lua_State* L);// получить здоровье педа.
int setpedhealth(lua_State* L);// установить здоровье педа.

int getpedarmour(lua_State* L); // получить броню педа.
int setarmour(lua_State* L);// установить броню педа.
int getcarhealth(lua_State* L); // получить кол-во здоровья авто.
int setcarhealth(lua_State* L); // установить здоровье авто.

int loadmodel(lua_State* L);// загрузить модель.
int createcar(lua_State* L);// создать авто на координатах на координатах.
int availablemodel(lua_State* L);// проверка на загруженность модели.
int releasemodel(lua_State* L);// удалить модель из памяти.

int keypress(lua_State* L);// проверка на нажатия клавиши.
int lockstatus(lua_State* L);// статус двери авто.
int givemoney(lua_State* L);// дать денег.
int getpedcoordes(lua_State* L);// получить координаты.

int printmessage(lua_State* L);// вывести сообщение на экран.
int randomfindped(lua_State* L);// найти случайнного педа.
int incar(lua_State* L);// пед в авто?
int exitcar(lua_State* L);// выйти из авто.

int createped(lua_State* L);// создать педа.
int create_marker_actor(lua_State* L);// создать маркер над педа.
int removemarker(lua_State* L);// удалить маркер.
int ped_sprint_to_point(lua_State* L);//пед делает спринт к точке.

int ped_walk_to_point(lua_State* L);// пед идет к точке.
int getpedangle(lua_State* L);// получить угол педа.
int load_requested_models(lua_State* L);// загрузить модель вне очереди.
int giveweaponped(lua_State* L);// дать педу оружие.

int ped_aim_at_ped(lua_State* L);// пед целиться в педе.
int is_current_weapon_ped(lua_State* L);// проверить текущее оружие.
int worldcoord(lua_State* L); // Перевод в мировые координаты.
int getpedcoordinates_on_x(lua_State* L); // Получить мировую координату по x для педа.

int getpedcoordinates_on_y(lua_State* L); // Получить мировую координату по y для педа.
int kill_ped_on_foot(lua_State* L);// убить педа пешком.
int kill_char_any_means(lua_State* L);// убить педа любыми средствами.

int create_sphere(lua_State* L); //создать сферу.
int remove_sphere(lua_State* L); // удалить сферу.
int remove_ped(lua_State* L); // удалить педа.
int remove_car(lua_State* L); // удалить авто.

int car_in_water(lua_State* L); // проверка авто в воде?.
int set_wanted(lua_State* L); // уcтановить уровень розыска.
int clear_wanted(lua_State* L);// убрать уровень розыска.
int kill_ped(lua_State* L); // убить педа.

int setpedcoordes(lua_State* L); // установить координаты для педа.
int ped_in_point_in_radius(lua_State* L); // проверить находится пед в координатах с радиусом.
int cardrive(lua_State* L); // авто едет в точку.

int setcarspeed(lua_State* L); // установить скорость авто.
int getflagmission(lua_State* L); // проверка флага миссии.
int setflagmission(lua_State* L); // уcтановить флага миссии.
int showtext(lua_State* L);// Вывод особого текста на экран.

int remove_blip(lua_State* L);// удалить метку с карты.
int createblip(lua_State* L); // создать метку на карте.
int play_sound(lua_State* L);// проиграть мелодию.
int isped(lua_State* L); // проверка это пед?.

int isvehicle(lua_State* L); // проверка это транспорт?.
int opendoorcar(lua_State* L); // открыть дверь авто.
int randomfindcar(lua_State* L); //Найти случайное авто.
int create_money_pickup(lua_State* L); //создать пачку денег.

int getcarcoordes(lua_State* L); // получить координаты авто.
int getcarcoordinates_on_x(lua_State* L);// Получить мировую координату по x для авто.
int getcarcoordinates_on_y(lua_State* L); // Получить мировую координату по y для авто.
int car_in_point_in_radius(lua_State* L); // проверить находится авто в координатах с радиусом.

int setdrivingstyle(lua_State* L); // установить стиль езды авто.
int findped(lua_State* L); // найти педа в пуле.
int create_weapon_pickup(lua_State* L); //создать пикап оружие.
int create_pickup(lua_State* L); //создать пикап.

int picked_up(lua_State* L); // проверка пикап подобран.
int remove_pickup(lua_State* L); // удалить пикап.
int play_voice(lua_State* L); // Проиграть голос(реплику).
int fade(lua_State* L);//затенение, просветления.

int draw_corona(lua_State* L); // создать корону(гоночный чекпойнт).
int sound_coordinate(lua_State* L); // Проиграть звук в координатах
int show_text_styled(lua_State* L); // вывести игровой текст.
int setcarangle(lua_State* L); // установить угол авто.

int createmarker(lua_State* L); // создать маркер на карте.
int setsizemarker(lua_State* L); // установить размер отображение маркера на карте.
int checkcheat(lua_State* L); // чит код введен?.
int destroy(lua_State* L); // удаления объектов из памяти при перезагрузки скрипта.

int my_yield(lua_State* L); // приостановить выполнение скрипта.
int setcardrive(lua_State* L); //  установить водителя для авто.
int setcarpassenger(lua_State* L); // установить пассажира для авто.
int setcarfirstcolor(lua_State* L); // установить первый цвет авто.

int setcarseconscolor(lua_State* L);// установить второй цвет авто.
int set_traffic(lua_State* L); // установить плотность трафика транспорта.
int create_marker_car(lua_State* L); //создать маркер над авто.
int car_explode(lua_State* L); // взорвать авто.

int is_car_stopped(lua_State* L); // авто остановилось.
int create_explosion(lua_State* L); // Создать взрыв на координатах.
int set_status_engine(lua_State* L); // установить состояние двигателя авто.
int player_defined(lua_State* L); // пед существует?

int setclock(lua_State* L); // задать время.
int arrested(lua_State* L); // пед арестован?
int create_marker_pickup(lua_State* L);// создать маркер над пикапом.
int createobj(lua_State* L); // создать объект.

int remove_obj(lua_State* L); // удалить объект.
int getobjcoordes(lua_State* L); // получить координаты объекта.
int create_marker_obj(lua_State* L); //создать маркер над объектом.

int isobject(lua_State* L); // проверка это объект?.
int setpedangle(lua_State* L); // установить угол педа.
int setcaraction(lua_State* L);// установить поведение авто.
int move_obj(lua_State* L); //двигать объект.

int move_rotate(lua_State* L); // вращать объект.
int getobjangle(lua_State* L); // получить угол объекта.
int findcar(lua_State* L); // Найти авто.
int setcartask(lua_State* L);// установить задачу авто.

int setcarcoordes(lua_State* L);// установить координаты авто.
int is_car_stuck(lua_State* L);//03CE: car 12@ stuck если машина застряла.
int is_car_upsidedown(lua_State* L); //01F4: car 12@ flipped если машина перевернута.
int is_car_upright(lua_State* L); // 020D: car 12@ flipped если указанный автомобиль перевернут.

int find_road_for_car(lua_State* L); // найти дорогу.
int setcarstrong(lua_State* L); // сделать авто устойчивым.
int putincar(lua_State* L);// переместить педа в авто.
int game_font_print(lua_State* L); // вывести особенный игровой текст.

int star_timer(lua_State* L); // включить таймер.
int stop_timer(lua_State* L); // остановить таймер.

int timer_donw(lua_State* L); //  таймер на уменьшение.
int ped_attack_car(lua_State* L); // пед атакует авто.
int ped_frozen(lua_State* L); // заморозить игpока.
int hold_cellphone(lua_State* L); // поднять телефон.

int car_lastweapondamage(lua_State* L); // номер оружие, которое нанесло урон авто.
int car_currentgear(lua_State* L); // текущая передача авто.
int getcar_model(lua_State* L); // получить модель авто.
int setcarsiren(lua_State* L); // установить сирену для авто.

int ped_car_as_driver(lua_State* L); // пед садится в авто как водитель.
int ped_car_as_passenger(lua_State* L); // пед садится в авто как пассажир.
int show_text_gtx(lua_State* L); // вывести игровой текст.
int camera_at_point(lua_State* L); //переместить камеру в координатах.

int set_camera_position(lua_State* L); //установить камеру в координатах.
int restore_camera(lua_State* L); // восстановить камеру.
int is_wanted_level(lua_State* L); // проверить уровень розыска.
int ped_atack(lua_State* L); // пед бьет.

int flash_hud(lua_State* L); // Мигание элементов HUD.
int set_radio(lua_State* L); // установить радио.
int set_car_tires(lua_State* L); // проколоть шину авто.

int create_spec_ped(lua_State* L); // создать спец педа.
int set_wheel_status(lua_State* L); // установить состояния шин авто.
int set_skin(lua_State* L); // установить скин педа.
int remove_spec_ped(lua_State* L);  // удалить спец педа.

int go_to_route(lua_State* L); //установить маршрут авто.
int add_stuck_car_check(lua_State* L); // условия для того, чтобы авто считалась застрявшей.
int load_scene(lua_State* L); // загрузить модели на координатах заранее.
int ped_anim(lua_State* L); // анимация.

int del_anim(lua_State* L); // удалить анимацию.
int get_current_name_luascript(lua_State* L); // получить имя текущего lua файла.
int star_mission_marker(lua_State* L); // создать маркер для миссии.
int getobjcoordinates_on_x(lua_State* L); // Получить мировую координату по x для объекта.

int getobjcoordinates_on_y(lua_State* L); // Получить мировую координату по y для объекта.
int set_widescreen(lua_State* L); // вкл/выкл широкий экран.
int follow_the_leader(lua_State* L); //01DE / 01DF	следовать за лидером.
int getcarspeed(lua_State* L); // получить скорость авто.

int Getcameracoordes(lua_State* L);// получить координаты камеры.
int remove_all_weapons_ped(lua_State* L); // удалить все оружия педа.
int Getweaponslot(lua_State* L); // получить номер слота по типу оружия.
int get_damage_weapon_ped(lua_State* L); // получить последний урон педа от оружия.

int get_aimgun_ped(lua_State* L); // получить показатель цели педа.
int get_ammo_weapon_ped(lua_State* L);// получить кол-во патроны текущего оружие педа.
int createfireonped(lua_State* L);// создать огонь на педе.
int createfireoncar(lua_State* L);// создать огонь на авто.

int createfireoncords(lua_State* L); // создать огонь на координатах.
int remove_fire(lua_State* L); // удалить огонь.
int ped_shutdown(lua_State* L);
int is_ped_damage_from_ped(lua_State* L); // получил ли пед урон от педа.

int is_targetting_in_ped(lua_State* L); // игрок целиться в педа.
int Remove_weapon_model(lua_State* L); // удалить оружие у педа.
int Createped(lua_State* L); // макрос создать педа.
int Createcar(lua_State* L); // макрос создать авто на координатах.

int Giveweaponped(lua_State* L); // макрос дать педу оружие и патроны.
int Opendoorcar(lua_State* L); // Макрос открыть все двери авто.
int Create_weapon_pickup(lua_State* L); // макрос создать пикап оружие.
int Create_pickup(lua_State* L); // макрос создать пикап.

int Get_model_and_type_current_weapon_ped(lua_State* L); // макрос получить модель и тип текущего оружие педа.
int is_ped_in_car(lua_State* L); // игрок в авто?
int ped_car(lua_State* L); // авто педа.
int wanted_level(lua_State* L); // получить уровень розыска.

int get_model_current_weapon_ped(lua_State* L); // макрос получить модель текущего оружие педа.
int get_type_current_weapon_ped(lua_State* L); // макрос получить тип текущего оружие педа.
int set_camera_and_point(lua_State* L); // установить и переместить камеру в координатах.
int get_damage_current_weapon(lua_State* L); // получить уровень урона текущего оружие.

int set_damage_current_weapon(lua_State* L); // установить уровень урона текущего оружие педа.
int set_clip_current_weapon(lua_State* L); // установить объём обоймы текущего оружие педа (только огнестрельного).
int ped_in_targetting(lua_State* L); // пед на прицеле.
int heli_set_orientation(lua_State* L); // транспорт/краны/зоны
int heli_clear_orientation(lua_State* L); // транспорт/краны/зоны
int heli_set_stabiliser(lua_State* L); // транспорт/краны/зоны
int heli_make_crashing_down(lua_State* L); // транспорт/краны/зоны
int heli_is_player_in_any_heli(lua_State* L); // транспорт/краны/зоны
int heli_is_char_in_any_heli(lua_State* L); // транспорт/краны/зоны
int heli_switch(lua_State* L); // транспорт/краны/зоны
int catalina_heli_start(lua_State* L); // транспорт/краны/зоны
int catalina_heli_take_off(lua_State* L); // транспорт/краны/зоны
int catalina_heli_remove(lua_State* L); // транспорт/краны/зоны
int catalina_heli_grab(lua_State* L); // транспорт/краны/зоны
int catalina_heli_fly_away(lua_State* L); // транспорт/краны/зоны
int catalina_heli_has_shot_down(lua_State* L); // транспорт/краны/зоны
int plane_goto_coords(lua_State* L); // транспорт/краны/зоны
int plane_is_player_in_any_plane(lua_State* L); // транспорт/краны/зоны
int plane_is_char_in_any_plane(lua_State* L); // транспорт/краны/зоны
int find_drug_plane_coords(lua_State* L); // транспорт/краны/зоны
int find_drop_off_plane_coords(lua_State* L); // транспорт/краны/зоны
int has_drug_plane_shot_down(lua_State* L); // транспорт/краны/зоны
int has_drop_off_plane_shot_down(lua_State* L); // транспорт/краны/зоны
int boat_goto_coords(lua_State* L); // транспорт/краны/зоны
int boat_set_cruise_speed(lua_State* L); // транспорт/краны/зоны
int boat_anchor(lua_State* L); // транспорт/краны/зоны
int boat_start_foam_animation(lua_State* L); // транспорт/краны/зоны
int boat_update_foam_animation(lua_State* L); // транспорт/краны/зоны
int boat_is_player_in_any_boat(lua_State* L); // транспорт/краны/зоны
int boat_is_char_in_any_boat(lua_State* L); // транспорт/краны/зоны
int is_vehicle_boat(lua_State* L); // транспорт/краны/зоны
int train_catch_char_obj(lua_State* L); // транспорт/краны/зоны
int train_catch_coll_obj(lua_State* L); // транспорт/краны/зоны
int rc_give_remote_controlled_car_to_player(lua_State* L); // транспорт/краны/зоны
int rc_give_remote_controlled_model_to_player(lua_State* L); // транспорт/краны/зоны
int rc_get_remote_controlled_car(lua_State* L); // транспорт/краны/зоны
int rc_buggy_blow_up(lua_State* L); // транспорт/краны/зоны
int rc_buggy_remove(lua_State* L); // транспорт/краны/зоны
int rc_detonate_enable(lua_State* L); // транспорт/краны/зоны
int rc_detonate_on_contact_enable(lua_State* L); // транспорт/краны/зоны
int player_is_in_remote_mode(lua_State* L); // транспорт/краны/зоны
int set_fade_jumpcut_after_rc_explosion(lua_State* L); // транспорт/краны/зоны
int bike_is_player_on_any_bike(lua_State* L); // транспорт/краны/зоны
int bike_is_char_on_any_bike(lua_State* L); // транспорт/краны/зоны
int is_char_in_flying_vehicle(lua_State* L); // транспорт/краны/зоны
int is_player_in_flying_vehicle(lua_State* L); // транспорт/краны/зоны
int crane_activate(lua_State* L); // транспорт/краны/зоны
int crane_is_carrying_car(lua_State* L); // транспорт/краны/зоны
int crane_is_targetting_car(lua_State* L); // транспорт/краны/зоны
int escalators_count(lua_State* L); // транспорт/краны/зоны
int zone_find_index(lua_State* L); // транспорт/краны/зоны
int zone_get_level_from_position(lua_State* L); // транспорт/краны/зоны
// ================= ЭТАП 12: Остальной транспорт GTA VC =================

// КРАНЫ
int crusher_crane_activate(lua_State* L);
int military_crane_activate(lua_State* L);
int crane_deactivate(lua_State* L);
int crane_military_collected_all(lua_State* L);
int crane_is_lifting_car(lua_State* L);

// КАТЕРА
int boat_stop(lua_State* L);

// ВЕРТОЛЕТЫ
int heli_goto_coords(lua_State* L);
int heli_create_dust_effect(lua_State* L);

// УПРАВЛЕНИЕ АВТО
int car_goto_accurate(lua_State* L);
int car_set_idle(lua_State* L);
int car_wander_randomly(lua_State* L);
int car_face_coord(lua_State* L);
int freeze_car_position(lua_State* L);
int freeze_car_no_collide(lua_State* L);
int set_car_status(lua_State* L);
int set_car_mission(lua_State* L);
int car_block_car(lua_State* L);
int car_ram_car(lua_State* L);
int car_change_lane(lua_State* L);
int car_drive_straight(lua_State* L);
int set_car_forward_speed(lua_State* L);
int car_wait(lua_State* L);
int car_route_seed(lua_State* L);
int car_straight_dist(lua_State* L);
int car_convoy(lua_State* L);
int car_make_stronger(lua_State* L);
int car_to_player(lua_State* L);
int car_fade_in(lua_State* L);
int car_on_trailer(lua_State* L);

// СВОЙСТВА АВТО
int set_car_visible(lua_State* L);
int set_car_watertight(lua_State* L);
int set_car_big_wheels(lua_State* L);
int set_car_bproof(lua_State* L);
int set_car_fproof(lua_State* L);
int set_car_rproof(lua_State* L);
int set_car_no_collide(lua_State* L);
int set_car_burst_tires(lua_State* L);
int set_car_dmg_player_only(lua_State* L);
int car_load_collision(lua_State* L);
int car_upside_safe(lua_State* L);
int car_change_color(lua_State* L);
int car_set_comp(lua_State* L);

// ЗДОРОВЬЕ И УГОЛ
int set_car_health(lua_State* L);
int get_car_health(lua_State* L);
int get_car_heading(lua_State* L);

// ВЕКТОРА
int get_car_forward_x(lua_State* L);
int get_car_forward_y(lua_State* L);

// ПРОВЕРКИ АВТО (boolean)
int is_car_in_air(lua_State* L);
int is_car_in_air_proper(lua_State* L);
int car_health_greater(lua_State* L);
int is_car_crushed(lua_State* L);
int is_car_remap(lua_State* L);
int is_car_stuck_roof(lua_State* L);
int car_is_tire_burst(lua_State* L);
int is_car_visibly_damaged(lua_State* L);
int car_waiting_collision(lua_State* L);
int car_is_door_closed(lua_State* L);
int is_car_on_trailer(lua_State* L);
int car_damaged_by_car(lua_State* L);
int car_damaged_by_ped(lua_State* L);
int car_just_sunk(lua_State* L);
int car_drowning(lua_State* L);
int car_has_weapon(lua_State* L);
int car_any_bomb(lua_State* L);
int car_has_bomb(lua_State* L);
int is_car_in_water(lua_State* L);
int fire_button_pressed(lua_State* L);
int is_car_alive(lua_State* L);
int does_vehicle_exist(lua_State* L);
int is_car_in_garage(lua_State* L);
int was_ever_police(lua_State* L);

// ОЧИСТКА
int clear_car_dmg(lua_State* L);
int clear_car_wep_dmg(lua_State* L);
int remove_stuck_check(lua_State* L);
int remove_upside_check(lua_State* L);
int add_upside_check(lua_State* L);
int unlock_doors_area(lua_State* L);

// ДВЕРИ/БАГАЖНИК
int close_all_doors(lua_State* L);
int close_car_boot(lua_State* L);
int pop_car_boot(lua_State* L);
int pop_car_boot_physics(lua_State* L);
int lock_doors(lua_State* L);
int change_car_lock(lua_State* L);

// ПЕД/ВОДИТЕЛЬ
int get_car_driver_ref(lua_State* L);
int get_char_in_seat(lua_State* L);
int order_driver_out(lua_State* L);
int set_ped_max_drive_speed(lua_State* L);
int set_ped_drive_aggression(lua_State* L);
int shuffle_to_driver(lua_State* L);
int stay_when_jacked(lua_State* L);
int is_ped_in_police_car(lua_State* L);
int is_ped_touching_vehicle(lua_State* L);
int is_ped_leaving_to_die(lua_State* L);
int is_ped_drowning(lua_State* L);
int is_ped_in_water(lua_State* L);
int is_ped_in_zone(lua_State* L);
int is_player_in_zone(lua_State* L);

// ТЕЛЕПОРТАЦИЯ
int warp_ped_from_car(lua_State* L);
int warp_ped_passenger(lua_State* L);
int warp_player_from_car(lua_State* L);

// ОРУЖИЕ ТРАНСПОРТА
int set_car_ammo(lua_State* L);
int switch_car_weapons(lua_State* L);
int set_car_weapon(lua_State* L);
int fire_hunter_gun(lua_State* L);
int set_tank_detonate(lua_State* L);
int ensure_drive_by(lua_State* L);

// БОМБЫ/СИГНАЛИЗАЦИЯ
int arm_car_bomb(lua_State* L);
int disarm_car_bomb(lua_State* L);
int give_car_alarm(lua_State* L);

// ДОПОЛНИТЕЛЬНЫЕ
int switch_car_siren(lua_State* L);
int set_driving_style_op(lua_State* L);
int set_char_obj_wait(lua_State* L); // нет задач (ждёт на ногах)
int set_char_obj_flee_coord(lua_State* L); // убежать от точки пешком до безопасности
int set_char_obj_guard_spot(lua_State* L); // охранять точку
int set_char_obj_guard_area(lua_State* L); // охранять зону (радиус)
int set_char_obj_wait_in_car(lua_State* L); // ждать в машине
int set_char_obj_leave_car(lua_State* L); // выйти из машины
int set_char_obj_enter_car_passenger(lua_State* L); // сесть в авто как пассажир
int set_char_obj_enter_car_driver(lua_State* L); // сесть в авто как водитель
int set_char_obj_follow_car_in_car(lua_State* L); // следовать за авто из авто
int set_char_obj_destroy_object(lua_State* L); // уничтожить объект
int set_char_obj_goto_area_in_car(lua_State* L); // ехать к зоне
int set_char_obj_follow_car_foot_offset(lua_State* L); // следовать за авто пешком смещение
int set_char_obj_guard_attack(lua_State* L); // охрана — атаковать противника
int set_char_obj_goto_coord_on_foot(lua_State* L); // идти к координатам пешком
int set_char_obj_goto_coord_in_car(lua_State* L); // ехать к координатам
int set_char_obj_run_to_coord(lua_State* L); // бежать к координатам
int set_char_obj_goto_area_any_means(lua_State* L); // добираться любыми средствами
int set_char_obj_follow_formation(lua_State* L); // следовать в строю за лидером
int set_char_obj_hail_taxi(lua_State* L); // вызвать такси
int set_char_obj_steal_any_car(lua_State* L); // украсть любую машину
int set_char_obj_leave_any_car(lua_State* L); // выйти из любого авто
int set_char_obj_flee_car(lua_State* L); // убежать от машины
int set_char_obj_walk_to_char(lua_State* L); // идти к другому педу
int set_char_obj_steal_any_car_mission(lua_State* L); // украсть любую миссионную машину
int set_char_obj_sprint_to_coord(lua_State* L); // спринт к координатам
int set_char_obj_goto_car_on_foot(lua_State* L); // идти к машине пешком
int get_projectile_count(lua_State* L); // число активных снарядов
int get_projectile_weapon(lua_State* L); // тип оружия снаряда
int get_projectile_source(lua_State* L); // источник снаряда (указатель)
int get_projectile_pos(lua_State* L); // позиция снаряда x,y,z
int set_projectile_pos(lua_State* L); // уст позицию снаряда (направление ракеты)
int is_projectile_in_use(lua_State* L); // слот снаряда занят
int remove_all_projectiles(lua_State* L); // удалить все снаряды
int is_projectile_in_area(lua_State* L); // снаряд в зоне (6 координат)
int destroy_projectiles_in_area(lua_State* L); // уничтожить снаряды в зоне
int add_projectile(lua_State* L); // создать снаряд (источник, оружие, x,y,z, сила)
int find_car_by_model(lua_State* L); // найти первое авто по модели
int find_ped_by_model(lua_State* L); // найти первого педа по модели
int find_object_by_model(lua_State* L); // найти объект по модели
int get_vehicle_pool_size(lua_State* L); // размер пула транспорта
int get_car_pool_index(lua_State* L); // индекс авто в пуле
int get_car_by_pool_index(lua_State* L); // авто по индексу в пуле
int get_car_velocity(lua_State* L); // скорость авто m_vecMoveSpeed (x,y,z)
int set_car_velocity(lua_State* L); // уст скорость авто m_vecMoveSpeed (x,y,z)
int remove_weapon_from_char(lua_State* L); // удалить оружие у педа (0x555)
int get_forced_weather_type(lua_State* L); // принудительный тип погоды
int is_lightning_burst(lua_State* L); // вспышка молнии (CWeather::LightningBurst)
int set_char_obj_kill_char_on_foot(lua_State* L); // убить педа пешком (ближний бой)
int set_char_obj_kill_char_any_means(lua_State* L); // убить педа любыми средствами
int set_char_obj_flee_char_on_foot_till_safe(lua_State* L); // убежать от педа до безопасного места
int set_char_obj_flee_char_on_foot_always(lua_State* L); // всегда убегать от педа
int set_char_obj_goto_char_on_foot(lua_State* L); // идти к педу пешком
int set_char_obj_aim_gun_at_char(lua_State* L); // целиться из оружия в педа
int set_char_obj_kill_player_on_foot(lua_State* L); // убить игрока пешком
int set_char_obj_kill_player_any_means(lua_State* L); // убить игрока любыми средствами
int set_char_obj_flee_player_on_foot_till_safe(lua_State* L); // убежать от игрока до безопасного места
int set_char_obj_flee_player_on_foot_always(lua_State* L); // всегда убегать от игрока
int set_char_obj_goto_player_on_foot(lua_State* L); // идти к игроку пешком
int set_char_obj_no_obj(lua_State* L); // снять задачу с педа
int set_char_obj_catch_train(lua_State* L); // сесть на поезд
int set_char_obj_buy_ice_cream(lua_State* L); // купить мороженое
int set_char_obj_goto_area_on_foot(lua_State* L); // идти к зоне пешком
int set_char_obj_destroy_car(lua_State* L); // уничтожить машину
int set_char_obj_fire_at_object_from_vehicle(lua_State* L); // стрелять по объекту из авто
int set_char_obj_follow_route(lua_State* L); // следовать по маршруту
int get_heli_forced_z(lua_State* L); // принудительная высота вертолёта (зависание)
int set_heli_forced_z(lua_State* L); // уст. принудительная высота вертолёта (зависание)
int get_heli_searchlight_x(lua_State* L); // координата X прожектора вертолёта
int set_heli_searchlight_x(lua_State* L); // уст. координата X прожектора вертолёта
int get_heli_searchlight_y(lua_State* L); // координата Y прожектора вертолёта
int set_heli_searchlight_y(lua_State* L); // уст. координата Y прожектора вертолёта
int get_heli_searchlight_intensity(lua_State* L); // яркость прожектора вертолёта (0..1)
int set_heli_searchlight_intensity(lua_State* L); // уст. яркость прожектора вертолёта (0..1)
int get_heli_rotor_angle(lua_State* L); // угол вращения винта вертолёта
int set_heli_rotor_angle(lua_State* L); // уст. угол вращения винта вертолёта
int heli_send_down_swat(lua_State* L); // спецназ спускается с вертолёта на верёвке
int heli_get_num_random(lua_State* L); // число случайных вертолётов
int heli_set_num_random(lua_State* L); // уст. число случайных вертолётов
int heli_is_catalina_on(lua_State* L); // вертолёт Катарины активен
int heli_set_catalina_on(lua_State* L); // вкл/выкл вертолёт Катарины
int get_ped_velocity(lua_State* L); // скорость педа m_vecMoveSpeed (x,y,z)
int set_ped_velocity(lua_State* L); // уст. скорость педа m_vecMoveSpeed (x,y,z)
int get_object_velocity(lua_State* L); // скорость объекта m_vecMoveSpeed (x,y,z)
int set_object_velocity(lua_State* L); // уст. скорость объекта m_vecMoveSpeed (x,y,z)
int get_car_speed(lua_State* L); // скалярная скорость авто (м/с)
int get_ped_speed(lua_State* L); // скалярная скорость педа (м/с)
int get_object_speed(lua_State* L); // скалярная скорость объекта (м/с)
int add_ammo_to_char(lua_State* L); // добавить патронов педу в оружие
int set_char_ammo(lua_State* L); // установить патроны педу в оружие
int get_ammo_in_char_weapon(lua_State* L); // патроны в оружии педа
int get_char_weapon_in_slot(lua_State* L); // оружие в слоте педа
int set_player_fast_reload(lua_State* L); // быстрая перезарядка игроку
int is_char_shooting_in_area(lua_State* L); // пед стреляет в зоне
int set_char_stop_shoot_dont_seek_entity(lua_State* L); // пед перестаёт стрелять и не ищет цель
int add_bullet_trace_radius(lua_State* L); // след пули: от точки к точке (радиус, время, прозрачность)
int add_bullet_trace_weapon(lua_State* L); // след пули: оружием и сущностью
int get_bullet_trace_count(lua_State* L); // число активных следов пуль
int get_bullet_trace_origin(lua_State* L); // точка начала следа пули
int get_bullet_trace_target(lua_State* L); // точка конца следа пули
int clear_bullet_traces(lua_State* L); // удалить все следы пуль
int place_3d_marker(lua_State* L); // 3D-маркер (стрелка/цилиндр)
int place_3d_marker_set(lua_State* L); // набор 3D-маркеров
int get_num_active_3d_markers(lua_State* L); // число активных 3D-маркеров
int get_pedstat_m_fFleeDistance(lua_State* L); // дистанция бегства педа
int set_pedstat_m_fFleeDistance(lua_State* L); // уст. дистанция бегства педа
int get_pedstat_m_fHeadingChangeRate(lua_State* L); // скорость поворота педа
int set_pedstat_m_fHeadingChangeRate(lua_State* L); // уст. скорость поворота педа
int get_pedstat_m_fAttackStrength(lua_State* L); // сила атаки педа
int set_pedstat_m_fAttackStrength(lua_State* L); // уст. сила атаки педа
int get_pedstat_m_fDefendWeakness(lua_State* L); // слабость защиты педа
int set_pedstat_m_fDefendWeakness(lua_State* L); // уст. слабость защиты педа
int get_car_door_status(lua_State* L); // состояние двери авто
int set_car_door_status(lua_State* L); // уст. состояние двери авто
int get_car_panel_status(lua_State* L); // состояние панели авто
int get_car_light_status(lua_State* L); // состояние фары авто
int set_car_light_status(lua_State* L); // уст. состояние фары авто
int get_car_wheel_status(lua_State* L); // состояние колеса авто
int set_car_wheel_status(lua_State* L); // уст. состояние колеса авто
int get_car_engine_status(lua_State* L); // состояние двигателя авто
int set_car_engine_status(lua_State* L); // уст. состояние двигателя авто
int apply_car_damage(lua_State* L); // повредить компонент авто
int fuck_car_completely(lua_State* L); // уничтожить авто визуально
int reset_car_damage(lua_State* L); // сбросить повреждения авто
int add_AutoPaintingBudget(lua_State* L); // бюджет на покраску авто
int add_PropertyBudget(lua_State* L); // бюджет на недвижимость
int add_FashionBudget(lua_State* L); // бюджет на одежду
int add_WeaponBudget(lua_State* L); // бюджет на оружие
int add_BloodRingKills(lua_State* L); // убийства в Bloodring
int add_PizzasDelivered(lua_State* L); // доставленные пиццы
int add_Assassinations(lua_State* L); // заказные убийства
int add_MovieStunts(lua_State* L); // киноскунты
int add_LoanSharks(lua_State* L); // визиты к ростовщикам
int clear_all_char_anims(lua_State* L); // очистить все анимации педа
int clear_char_follow_path(lua_State* L); // очистить маршрут педа
int clear_char_last_damage_entity(lua_State* L); // забыть последнюю повредившую сущность
int clear_char_last_weapon_damage(lua_State* L); // забыть последнее оружие урона
int char_set_idle(lua_State* L); // пед в режим ожидания
int add_armour_to_char(lua_State* L); // добавить брони педу
int add_armour_to_player(lua_State* L); // добавить брони игроку
int add_ammo_to_player(lua_State* L); // добавить патронов игроку
int clear_area_of_cars(lua_State* L); // очистить зону от машин
int clear_area_of_chars(lua_State* L); // очистить зону от педов
int get_population_total_peds(lua_State* L); // всего педов в мире
int get_population_civ_peds(lua_State* L); // мирных педов
int get_population_gang_peds(lua_State* L); // бандитских педов
int get_population_car_passengers(lua_State* L); // пассажиров в авто
int get_population_mission_peds(lua_State* L); // миссионных педов
int get_population_civ_male(lua_State* L); // мужчин-мирных
int get_population_civ_female(lua_State* L); // женщин-мирных
int get_population_max_in_use(lua_State* L); // макс. число используемых педов
int get_population_countdown(lua_State* L); // обратный отсчёт до спавна педов
int get_population_all_random_type(lua_State* L); // тип всех случайных педов
int set_population_all_random_type(lua_State* L); // уст. тип всех случайных педов
int get_hud_m_WantedTimer(lua_State* L); // таймер розыска на HUD
int set_hud_m_WantedTimer(lua_State* L); // таймер розыска на HUD
int get_hud_m_WeaponFadeTimer(lua_State* L); // таймер затухания оружия на HUD
int set_hud_m_WeaponFadeTimer(lua_State* L); // таймер затухания оружия на HUD
int get_hud_m_EnergyLostFadeTimer(lua_State* L); // таймер затухания потери энергии
int set_hud_m_EnergyLostFadeTimer(lua_State* L); // таймер затухания потери энергии
int get_hud_m_DisplayScoreFadeTimer(lua_State* L); // таймер затухания счёта
int set_hud_m_DisplayScoreFadeTimer(lua_State* L); // таймер затухания счёта
int get_hud_wanted_state(lua_State* L); // состояние розыска на HUD
int get_hud_help_message_state(lua_State* L); // состояние подсказки на HUD
int get_player_taxi_timer(lua_State* L); // таймер такси игрока
int get_player_death_fade_timer(lua_State* L); // таймер затухания при смерти
int get_player_2wheels_time(lua_State* L); // время на двух колёсах (мс)
int get_player_2wheels_distance(lua_State* L); // дистанция на двух колёсах
int get_player_road_density(lua_State* L); // плотность дорог вокруг игрока
int set_player_road_density(lua_State* L); // уст. плотность дорог вокруг игрока
int get_pedik_m_fYaw(lua_State* L); // угол наклона головы педа
int set_pedik_m_fYaw(lua_State* L); // уст. угол наклона головы педа
int get_pedik_m_fPitch(lua_State* L); // наклон головы педа по тангажу
int set_pedik_m_fPitch(lua_State* L); // уст. наклон головы педа по тангажу
int get_boat_m_fPropRotation(lua_State* L); // вращение винта лодки
int set_boat_m_fPropRotation(lua_State* L); // уст. вращение винта лодки
int get_boat_m_fPropSpeed(lua_State* L); // скорость винта лодки
int set_boat_m_fPropSpeed(lua_State* L); // уст. скорость винта лодки
int get_boat_m_fForcedZRotation(lua_State* L); // принудительный поворот лодки по Z
int set_boat_m_fForcedZRotation(lua_State* L); // уст. принудительный поворот лодки по Z
int get_boat_m_fBoatGasPedal(lua_State* L); // газ лодки
int set_boat_m_fBoatGasPedal(lua_State* L); // уст. газ лодки
int get_boat_m_fBoatSteeringLeftRight(lua_State* L); // руль лодки
int set_boat_m_fBoatSteeringLeftRight(lua_State* L); // уст. руль лодки
int get_bike_anim_group(lua_State* L); // группа анимаций мотоцикла
int set_bike_anim_group(lua_State* L); // уст. группу анимаций мотоцикла
int get_bike_damage_flags(lua_State* L); // флаги повреждений мотоцикла
int set_bike_damage_flags(lua_State* L); // уст. флаги повреждений мотоцикла
int get_time_step_non_clipped(lua_State* L); // шаг времени без клиппинга
int get_previous_time_ms(lua_State* L); // предыдущее время в мс
int get_time_ms_non_clipped(lua_State* L); // время в мс без клиппинга
int is_slow_motion_active(lua_State* L); // активен ли слоумоушен
int get_cycles_per_millisecond(lua_State* L); // циклы на миллисекунду
int get_clock_hours(lua_State* L); // часы игрового таймера
int get_clock_minutes(lua_State* L); // минуты игрового таймера
int get_clock_seconds(lua_State* L); // секунды игрового таймера
int get_clock_stored_hours(lua_State* L); // сохранённые часы
int get_clock_stored_minutes(lua_State* L); // сохранённые минуты
int get_clock_minutes_until(lua_State* L); // минут до заданного времени
int restore_clock(lua_State* L); // восстановить сохранённое время
int get_streaming_disabled(lua_State* L); // стриминг отключён
int set_streaming_disabled(lua_State* L); // отключить стриминг моделей
int get_streaming_models_requested(lua_State* L); // запрошено моделей
int get_streaming_priority_requests(lua_State* L); // приоритетных запросов
int get_streaming_image_size(lua_State* L); // размер образа стриминга
int get_streaming_buffer_size(lua_State* L); // размер буфера стриминга
int get_streaming_current_ped_grp(lua_State* L); // текущая группа педов
int get_streaming_loading_big_model(lua_State* L); // загружается большая модель
int get_streaming_peds_loaded(lua_State* L); // загружено педов
int get_streaming_channel_error(lua_State* L); // ошибка канала стриминга
int get_streaming_last_image_read(lua_State* L); // последний прочитанный образ
int get_streaming_old_sector_x(lua_State* L); // старый сектор X
int get_streaming_old_sector_y(lua_State* L); // старый сектор Y
int get_zone_level(lua_State* L); // остров по координатам
int find_information_zone(lua_State* L); // индекс информационной зоны
int get_navigation_zone(lua_State* L); // навигационная зона по индексу
int init_zones(lua_State* L); // инициализировать зоны
int traffic_lights_for_peds(lua_State* L); // светофор для пешеходов
int traffic_scan_for_lights(lua_State* L); // сканировать светофоры
int traffic_should_car_stop_for_bridge(lua_State* L); // машина должна остановиться у моста
int should_car_stop_for_light(lua_State* L); // должна ли машина остановиться на свет
int add_gun_flash_big(lua_State* L); // вспышка выстрела крупного оружия
int initialise_weapons(lua_State* L); // инициализировать оружие
int shutdown_weapons(lua_State* L); // завершить работу оружия
int update_weapons(lua_State* L); // обновить оружие
int do_doom_aiming(lua_State* L); // аим-траверс для оружия
int create_some_money(lua_State* L); // создать деньги на координатах
int give_player_goodies(lua_State* L); // выдать игроку бонусы по модели
int do_pickup_effects(lua_State* L); // эффект поднятия пикапа
int do_money_effects(lua_State* L); // эффект денег
int do_mine_effects(lua_State* L); // эффект мины
int get_obj_m_fAttachForce(lua_State* L); // сила привязки объекта
int set_obj_m_fAttachForce(lua_State* L); // уст. сила привязки объекта
int get_obj_m_CollisionDamageType(lua_State* L); // тип урона от столкновений
int set_obj_m_CollisionDamageType(lua_State* L); // уст. тип урона от столкновений
int get_obj_m_nSpecialCollisionType(lua_State* L); // специальный тип столкновений
int set_obj_m_nSpecialCollisionType(lua_State* L); // уст. специальный тип столкновений
int get_obj_m_nBounceScore(lua_State* L); // счёт отскока объекта
int set_obj_m_nBounceScore(lua_State* L); // уст. счёт отскока объекта
int get_obj_m_dwObjectTimer(lua_State* L); // таймер объекта
int set_obj_m_dwObjectTimer(lua_State* L); // уст. таймер объекта
int get_radar_range(lua_State* L); // дальность радара
int set_radar_range(lua_State* L); // уст. дальность радара
int set_sun_blocked_by_clouds(lua_State* L); // уст. солнце закрыто облаками
int update_corona_coors(lua_State* L); // обновить координаты короны
int is_cutscene_running(lua_State* L); // катсцена идёт
int get_num_cutscene_objs(lua_State* L); // число объектов катсцены
int get_cutscene_time_ms(lua_State* L); // время катсцены в мс
int choose_police_car_model(lua_State* L); // выбрать модель полицейской машины
int generate_emergency_services(lua_State* L); // вызвать экстренные службы
int clear_interesting_vehicles(lua_State* L); // очистить список интересных машин
int drag_car_to_point(lua_State* L); // тащить машину к точке
int find_max_speed_in_traffic(lua_State* L); // макс. скорость машины в трафике
int get_num_bright_lights(lua_State* L); // число ярких огней
int get_car_m_nPrimaryColor(lua_State* L); // m_nPrimaryColor (CVehicle)
int set_car_m_nPrimaryColor(lua_State* L); // уст. m_nPrimaryColor (CVehicle)
int get_car_m_nSecondaryColor(lua_State* L); // m_nSecondaryColor (CVehicle)
int set_car_m_nSecondaryColor(lua_State* L); // уст. m_nSecondaryColor (CVehicle)
int get_car_m_wWantedStarsOnEnter(lua_State* L); // m_wWantedStarsOnEnter (CVehicle)
int set_car_m_wWantedStarsOnEnter(lua_State* L); // уст. m_wWantedStarsOnEnter (CVehicle)
int get_car_m_wMissionValue(lua_State* L); // m_wMissionValue (CVehicle)
int set_car_m_wMissionValue(lua_State* L); // уст. m_wMissionValue (CVehicle)
int get_car_m_nNumPassengers(lua_State* L); // m_nNumPassengers (CVehicle)
int set_car_m_nNumPassengers(lua_State* L); // уст. m_nNumPassengers (CVehicle)
int get_car_m_nNumGettingIn(lua_State* L); // m_nNumGettingIn (CVehicle)
int set_car_m_nNumGettingIn(lua_State* L); // уст. m_nNumGettingIn (CVehicle)
int get_car_m_nGettingInFlags(lua_State* L); // m_nGettingInFlags (CVehicle)
int set_car_m_nGettingInFlags(lua_State* L); // уст. m_nGettingInFlags (CVehicle)
int get_car_m_nGettingOutFlags(lua_State* L); // m_nGettingOutFlags (CVehicle)
int set_car_m_nGettingOutFlags(lua_State* L); // уст. m_nGettingOutFlags (CVehicle)
int get_car_m_nMaxPassengers(lua_State* L); // m_nMaxPassengers (CVehicle)
int set_car_m_nMaxPassengers(lua_State* L); // уст. m_nMaxPassengers (CVehicle)
int get_car_m_fSteerAngle(lua_State* L); // m_fSteerAngle (CVehicle)
int set_car_m_fSteerAngle(lua_State* L); // уст. m_fSteerAngle (CVehicle)
int get_car_m_fGasPedal(lua_State* L); // m_fGasPedal (CVehicle)
int set_car_m_fGasPedal(lua_State* L); // уст. m_fGasPedal (CVehicle)
int get_car_m_fBreakPedal(lua_State* L); // m_fBreakPedal (CVehicle)
int set_car_m_fBreakPedal(lua_State* L); // уст. m_fBreakPedal (CVehicle)
int get_car_m_nCreatedBy(lua_State* L); // m_nCreatedBy (CVehicle)
int set_car_m_nCreatedBy(lua_State* L); // уст. m_nCreatedBy (CVehicle)
int get_car_m_nAmmoInClip(lua_State* L); // m_nAmmoInClip (CVehicle)
int set_car_m_nAmmoInClip(lua_State* L); // уст. m_nAmmoInClip (CVehicle)
int get_car_m_fHealth(lua_State* L); // m_fHealth (CVehicle)
int set_car_m_fHealth(lua_State* L); // уст. m_fHealth (CVehicle)
int get_car_m_nCurrentGear(lua_State* L); // m_nCurrentGear (CVehicle)
int set_car_m_nCurrentGear(lua_State* L); // уст. m_nCurrentGear (CVehicle)
int get_car_m_nTimeTillWeNeedThisCar(lua_State* L); // m_nTimeTillWeNeedThisCar (CVehicle)
int set_car_m_nTimeTillWeNeedThisCar(lua_State* L); // уст. m_nTimeTillWeNeedThisCar (CVehicle)
int get_car_m_nTimeOfDeath(lua_State* L); // m_nTimeOfDeath (CVehicle)
int set_car_m_nTimeOfDeath(lua_State* L); // уст. m_nTimeOfDeath (CVehicle)
int get_car_m_wBombTimer(lua_State* L); // m_wBombTimer (CVehicle)
int set_car_m_wBombTimer(lua_State* L); // уст. m_wBombTimer (CVehicle)
int get_car_m_nLastWeaponDamage(lua_State* L); // m_nLastWeaponDamage (CVehicle)
int set_car_m_nLastWeaponDamage(lua_State* L); // уст. m_nLastWeaponDamage (CVehicle)
int get_car_m_nRadioStation(lua_State* L); // m_nRadioStation (CVehicle)
int set_car_m_nRadioStation(lua_State* L); // уст. m_nRadioStation (CVehicle)
int get_car_m_bHornEnabled(lua_State* L); // m_bHornEnabled (CVehicle)
int set_car_m_bHornEnabled(lua_State* L); // уст. m_bHornEnabled (CVehicle)
int get_car_m_nSirenOrAlarm(lua_State* L); // m_nSirenOrAlarm (CVehicle)
int set_car_m_nSirenOrAlarm(lua_State* L); // уст. m_nSirenOrAlarm (CVehicle)
int get_car_m_nSirenExtra(lua_State* L); // m_nSirenExtra (CVehicle)
int set_car_m_nSirenExtra(lua_State* L); // уст. m_nSirenExtra (CVehicle)
int get_car_m_fSteerRatio(lua_State* L); // m_fSteerRatio (CVehicle)
int set_car_m_fSteerRatio(lua_State* L); // уст. m_fSteerRatio (CVehicle)
int get_car_m_nVehicleClass(lua_State* L); // m_nVehicleClass (CVehicle)
int set_car_m_nVehicleClass(lua_State* L); // уст. m_nVehicleClass (CVehicle)
int get_ped_m_fCollisionSpeed(lua_State* L); // m_fCollisionSpeed (CPed)
int set_ped_m_fCollisionSpeed(lua_State* L); // уст. m_fCollisionSpeed (CPed)
int get_ped_m_nGangFlags(lua_State* L); // m_nGangFlags (CPed)
int set_ped_m_nGangFlags(lua_State* L); // уст. m_nGangFlags (CPed)
int get_ped_m_nPedStatus(lua_State* L); // m_nPedStatus (CPed)
int set_ped_m_nPedStatus(lua_State* L); // уст. m_nPedStatus (CPed)
int get_ped_m_vecObjective(lua_State* L); // m_vecObjective (CPed) (x,y,z)
int set_ped_m_vecObjective(lua_State* L); // уст. m_vecObjective (CPed) (x,y,z)
int get_ped_m_fObjectiveAngle(lua_State* L); // m_fObjectiveAngle (CPed)
int set_ped_m_fObjectiveAngle(lua_State* L); // уст. m_fObjectiveAngle (CPed)
int get_ped_m_nPedFormation(lua_State* L); // m_nPedFormation (CPed)
int set_ped_m_nPedFormation(lua_State* L); // уст. m_nPedFormation (CPed)
int get_ped_m_nFearFlags(lua_State* L); // m_nFearFlags (CPed)
int set_ped_m_nFearFlags(lua_State* L); // уст. m_nFearFlags (CPed)
int get_ped_m_nEventType(lua_State* L); // m_nEventType (CPed)
int set_ped_m_nEventType(lua_State* L); // уст. m_nEventType (CPed)
int get_ped_m_fAngleToEvent(lua_State* L); // m_fAngleToEvent (CPed)
int set_ped_m_fAngleToEvent(lua_State* L); // уст. m_fAngleToEvent (CPed)
int get_ped_m_nAnimGroupId(lua_State* L); // m_nAnimGroupId (CPed)
int set_ped_m_nAnimGroupId(lua_State* L); // уст. m_nAnimGroupId (CPed)
int get_ped_m_vecOffsetSeek(lua_State* L); // m_vecOffsetSeek (CPed) (x,y,z)
int set_ped_m_vecOffsetSeek(lua_State* L); // уст. m_vecOffsetSeek (CPed) (x,y,z)
int get_ped_m_nActionTimer(lua_State* L); // m_nActionTimer (CPed)
int set_ped_m_nActionTimer(lua_State* L); // уст. m_nActionTimer (CPed)
int get_ped_m_nWaitState(lua_State* L); // m_nWaitState (CPed)
int set_ped_m_nWaitState(lua_State* L); // уст. m_nWaitState (CPed)
int get_ped_m_nWaitTimer(lua_State* L); // m_nWaitTimer (CPed)
int set_ped_m_nWaitTimer(lua_State* L); // уст. m_nWaitTimer (CPed)
int get_ped_m_wPathNodes(lua_State* L); // m_wPathNodes (CPed)
int set_ped_m_wPathNodes(lua_State* L); // уст. m_wPathNodes (CPed)
int get_ped_m_wCurPathNode(lua_State* L); // m_wCurPathNode (CPed)
int set_ped_m_wCurPathNode(lua_State* L); // уст. m_wCurPathNode (CPed)
int get_ped_m_nPathNodeTimer(lua_State* L); // m_nPathNodeTimer (CPed)
int set_ped_m_nPathNodeTimer(lua_State* L); // уст. m_nPathNodeTimer (CPed)
int get_ped_m_vecPathNextNode(lua_State* L); // m_vecPathNextNode (CPed) (x,y,z)
int set_ped_m_vecPathNextNode(lua_State* L); // уст. m_vecPathNextNode (CPed) (x,y,z)
int get_ped_m_fPathNextNodeDir(lua_State* L); // m_fPathNextNodeDir (CPed)
int set_ped_m_fPathNextNodeDir(lua_State* L); // уст. m_fPathNextNodeDir (CPed)
int get_ped_m_nPathNodeType(lua_State* L); // m_nPathNodeType (CPed)
int set_ped_m_nPathNodeType(lua_State* L); // уст. m_nPathNodeType (CPed)
int get_ped_m_fHealth(lua_State* L); // m_fHealth (CPed)
int set_ped_m_fHealth(lua_State* L); // уст. m_fHealth (CPed)
int get_ped_m_fArmour(lua_State* L); // m_fArmour (CPed)
int set_ped_m_fArmour(lua_State* L); // уст. m_fArmour (CPed)
int get_ped_m_nShadowUpdateTimer(lua_State* L); // m_nShadowUpdateTimer (CPed)
int set_ped_m_nShadowUpdateTimer(lua_State* L); // уст. m_nShadowUpdateTimer (CPed)
int get_ped_wRouteLastPoint(lua_State* L); // wRouteLastPoint (CPed)
int set_ped_wRouteLastPoint(lua_State* L); // уст. wRouteLastPoint (CPed)
int get_ped_wRoutePoints(lua_State* L); // wRoutePoints (CPed)
int set_ped_wRoutePoints(lua_State* L); // уст. wRoutePoints (CPed)
int get_ped_wRoutePos(lua_State* L); // wRoutePos (CPed)
int set_ped_wRoutePos(lua_State* L); // уст. wRoutePos (CPed)
int get_ped_wRouteType(lua_State* L); // wRouteType (CPed)
int set_ped_wRouteType(lua_State* L); // уст. wRouteType (CPed)
int get_ped_wRouteCurDir(lua_State* L); // wRouteCurDir (CPed)
int set_ped_wRouteCurDir(lua_State* L); // уст. wRouteCurDir (CPed)
int get_ped_m_fHeadingCurrent(lua_State* L); // m_fHeadingCurrent (CPed)
int set_ped_m_fHeadingCurrent(lua_State* L); // уст. m_fHeadingCurrent (CPed)
int get_ped_m_fHeadingGoal(lua_State* L); // m_fHeadingGoal (CPed)
int set_ped_m_fHeadingGoal(lua_State* L); // уст. m_fHeadingGoal (CPed)
int get_ped_m_fHeadingChangeRate(lua_State* L); // m_fHeadingChangeRate (CPed)
int set_ped_m_fHeadingChangeRate(lua_State* L); // уст. m_fHeadingChangeRate (CPed)
int get_ped_m_nEnterType(lua_State* L); // m_nEnterType (CPed)
int set_ped_m_nEnterType(lua_State* L); // уст. m_nEnterType (CPed)
int get_ped_m_nWalkAroundType(lua_State* L); // m_nWalkAroundType (CPed)
int set_ped_m_nWalkAroundType(lua_State* L); // уст. m_nWalkAroundType (CPed)
int get_ped_m_vecOffsetFromPhysSurface(lua_State* L); // m_vecOffsetFromPhysSurface (CPed) (x,y,z)
int set_ped_m_vecOffsetFromPhysSurface(lua_State* L); // уст. m_vecOffsetFromPhysSurface (CPed) (x,y,z)
int get_ped_vecSeekVehicle(lua_State* L); // vecSeekVehicle (CPed) (x,y,z)
int set_ped_vecSeekVehicle(lua_State* L); // уст. vecSeekVehicle (CPed) (x,y,z)
int get_ped_m_bInVehicle(lua_State* L); // m_bInVehicle (CPed)
int set_ped_m_bInVehicle(lua_State* L); // уст. m_bInVehicle (CPed)
int get_ped_m_fSeatPrecisionX(lua_State* L); // m_fSeatPrecisionX (CPed)
int set_ped_m_fSeatPrecisionX(lua_State* L); // уст. m_fSeatPrecisionX (CPed)
int get_ped_m_fSeatPrecisionY(lua_State* L); // m_fSeatPrecisionY (CPed)
int set_ped_m_fSeatPrecisionY(lua_State* L); // уст. m_fSeatPrecisionY (CPed)
int get_ped_m_nSeatType(lua_State* L); // m_nSeatType (CPed)
int set_ped_m_nSeatType(lua_State* L); // уст. m_nSeatType (CPed)
int get_ped_m_bHasPhone(lua_State* L); // m_bHasPhone (CPed)
int set_ped_m_bHasPhone(lua_State* L); // уст. m_bHasPhone (CPed)
int get_ped_m_wPhoneId(lua_State* L); // m_wPhoneId (CPed)
int set_ped_m_wPhoneId(lua_State* L); // уст. m_wPhoneId (CPed)
int get_ped_m_nLookingForPhone(lua_State* L); // m_nLookingForPhone (CPed)
int set_ped_m_nLookingForPhone(lua_State* L); // уст. m_nLookingForPhone (CPed)
int get_ped_m_nPhoneTalkTimer(lua_State* L); // m_nPhoneTalkTimer (CPed)
int set_ped_m_nPhoneTalkTimer(lua_State* L); // уст. m_nPhoneTalkTimer (CPed)
int get_ped_m_fFleeFromPosX(lua_State* L); // m_fFleeFromPosX (CPed)
int set_ped_m_fFleeFromPosX(lua_State* L); // уст. m_fFleeFromPosX (CPed)
int get_ped_m_fFleeFromPosY(lua_State* L); // m_fFleeFromPosY (CPed)
int set_ped_m_fFleeFromPosY(lua_State* L); // уст. m_fFleeFromPosY (CPed)
int get_ped_m_nFleeTimer(lua_State* L); // m_nFleeTimer (CPed)
int set_ped_m_nFleeTimer(lua_State* L); // уст. m_nFleeTimer (CPed)
int get_ped_m_nLastThreatTimer(lua_State* L); // m_nLastThreatTimer (CPed)
int set_ped_m_nLastThreatTimer(lua_State* L); // уст. m_nLastThreatTimer (CPed)
int get_ped_m_nStateUnused(lua_State* L); // m_nStateUnused (CPed)
int set_ped_m_nStateUnused(lua_State* L); // уст. m_nStateUnused (CPed)
int get_ped_m_nTimerUnused(lua_State* L); // m_nTimerUnused (CPed)
int set_ped_m_nTimerUnused(lua_State* L); // уст. m_nTimerUnused (CPed)
int get_ped_m_nAtchStoredWep(lua_State* L); // m_nAtchStoredWep (CPed)
int set_ped_m_nAtchStoredWep(lua_State* L); // уст. m_nAtchStoredWep (CPed)
int get_ped_m_nStoredGiveWep(lua_State* L); // m_nStoredGiveWep (CPed)
int set_ped_m_nStoredGiveWep(lua_State* L); // уст. m_nStoredGiveWep (CPed)
int get_ped_m_nStoredGiveAmmo(lua_State* L); // m_nStoredGiveAmmo (CPed)
int set_ped_m_nStoredGiveAmmo(lua_State* L); // уст. m_nStoredGiveAmmo (CPed)
int get_ped_m_nCurrentWeapon(lua_State* L); // m_nCurrentWeapon (CPed)
int set_ped_m_nCurrentWeapon(lua_State* L); // уст. m_nCurrentWeapon (CPed)
int get_ped_m_nWepSkills(lua_State* L); // m_nWepSkills (CPed)
int set_ped_m_nWepSkills(lua_State* L); // уст. m_nWepSkills (CPed)
int get_ped_m_nWeaponAccuracy(lua_State* L); // m_nWeaponAccuracy (CPed)
int set_ped_m_nWeaponAccuracy(lua_State* L); // уст. m_nWeaponAccuracy (CPed)
int get_ped_m_nBodyPart(lua_State* L); // m_nBodyPart (CPed)
int set_ped_m_nBodyPart(lua_State* L); // уст. m_nBodyPart (CPed)
int get_ped_m_vecHitLastPos(lua_State* L); // m_vecHitLastPos (CPed) (x,y,z)
int set_ped_m_vecHitLastPos(lua_State* L); // уст. m_vecHitLastPos (CPed) (x,y,z)
int get_ped_m_nHitCounter(lua_State* L); // m_nHitCounter (CPed)
int set_ped_m_nHitCounter(lua_State* L); // уст. m_nHitCounter (CPed)
int get_ped_m_nLastHitState(lua_State* L); // m_nLastHitState (CPed)
int set_ped_m_nLastHitState(lua_State* L); // уст. m_nLastHitState (CPed)
int get_ped_m_nFightFlags1(lua_State* L); // m_nFightFlags1 (CPed)
int set_ped_m_nFightFlags1(lua_State* L); // уст. m_nFightFlags1 (CPed)
int get_ped_m_nFightFlags2(lua_State* L); // m_nFightFlags2 (CPed)
int set_ped_m_nFightFlags2(lua_State* L); // уст. m_nFightFlags2 (CPed)
int get_ped_m_nFightFlags3(lua_State* L); // m_nFightFlags3 (CPed)
int set_ped_m_nFightFlags3(lua_State* L); // уст. m_nFightFlags3 (CPed)
int get_ped_m_nBleedCounter(lua_State* L); // m_nBleedCounter (CPed)
int set_ped_m_nBleedCounter(lua_State* L); // уст. m_nBleedCounter (CPed)
int get_ped_m_fLookDirection(lua_State* L); // m_fLookDirection (CPed)
int set_ped_m_fLookDirection(lua_State* L); // уст. m_fLookDirection (CPed)
int get_ped_m_nWepModelID(lua_State* L); // m_nWepModelID (CPed)
int set_ped_m_nWepModelID(lua_State* L); // уст. m_nWepModelID (CPed)
int get_ped_m_nLeaveCarTimer(lua_State* L); // m_nLeaveCarTimer (CPed)
int set_ped_m_nLeaveCarTimer(lua_State* L); // уст. m_nLeaveCarTimer (CPed)
int get_ped_m_nGetUpTimer(lua_State* L); // m_nGetUpTimer (CPed)
int set_ped_m_nGetUpTimer(lua_State* L); // уст. m_nGetUpTimer (CPed)
int get_ped_m_nLookTimer(lua_State* L); // m_nLookTimer (CPed)
int set_ped_m_nLookTimer(lua_State* L); // уст. m_nLookTimer (CPed)
int get_ped_m_nStandardTimer(lua_State* L); // m_nStandardTimer (CPed)
int set_ped_m_nStandardTimer(lua_State* L); // уст. m_nStandardTimer (CPed)
int get_ped_m_nAttackTimer(lua_State* L); // m_nAttackTimer (CPed)
int set_ped_m_nAttackTimer(lua_State* L); // уст. m_nAttackTimer (CPed)
int get_ped_m_nLastHitTime(lua_State* L); // m_nLastHitTime (CPed)
int set_ped_m_nLastHitTime(lua_State* L); // уст. m_nLastHitTime (CPed)
int get_ped_m_nHitRecoverTimer(lua_State* L); // m_nHitRecoverTimer (CPed)
int set_ped_m_nHitRecoverTimer(lua_State* L); // уст. m_nHitRecoverTimer (CPed)
int get_ped_m_nObjectiveTimer(lua_State* L); // m_nObjectiveTimer (CPed)
int set_ped_m_nObjectiveTimer(lua_State* L); // уст. m_nObjectiveTimer (CPed)
int get_ped_m_nDuckTimer(lua_State* L); // m_nDuckTimer (CPed)
int set_ped_m_nDuckTimer(lua_State* L); // уст. m_nDuckTimer (CPed)
int get_ped_m_nDuckAndCoverTimer(lua_State* L); // m_nDuckAndCoverTimer (CPed)
int set_ped_m_nDuckAndCoverTimer(lua_State* L); // уст. m_nDuckAndCoverTimer (CPed)
int get_ped_m_nBloodyTimer(lua_State* L); // m_nBloodyTimer (CPed)
int set_ped_m_nBloodyTimer(lua_State* L); // уст. m_nBloodyTimer (CPed)
int get_ped_m_nShotTime(lua_State* L); // m_nShotTime (CPed)
int set_ped_m_nShotTime(lua_State* L); // уст. m_nShotTime (CPed)
int get_ped_m_nShotTimeAdd(lua_State* L); // m_nShotTimeAdd (CPed)
int set_ped_m_nShotTimeAdd(lua_State* L); // уст. m_nShotTimeAdd (CPed)
int get_ped_m_nPanicCounter(lua_State* L); // m_nPanicCounter (CPed)
int set_ped_m_nPanicCounter(lua_State* L); // уст. m_nPanicCounter (CPed)
int get_ped_m_nDeadBleeding(lua_State* L); // m_nDeadBleeding (CPed)
int set_ped_m_nDeadBleeding(lua_State* L); // уст. m_nDeadBleeding (CPed)
int get_ped_m_nBodyPartBleeding(lua_State* L); // m_nBodyPartBleeding (CPed)
int set_ped_m_nBodyPartBleeding(lua_State* L); // уст. m_nBodyPartBleeding (CPed)
int get_ped_m_nNumNearPeds(lua_State* L); // m_nNumNearPeds (CPed)
int set_ped_m_nNumNearPeds(lua_State* L); // уст. m_nNumNearPeds (CPed)
int get_ped_m_nPedMoney(lua_State* L); // m_nPedMoney (CPed)
int set_ped_m_nPedMoney(lua_State* L); // уст. m_nPedMoney (CPed)
int get_ped_m_nLastDamWep(lua_State* L); // m_nLastDamWep (CPed)
int set_ped_m_nLastDamWep(lua_State* L); // уст. m_nLastDamWep (CPed)
int get_ped_m_vecAttachOffset(lua_State* L); // m_vecAttachOffset (CPed) (x,y,z)
int set_ped_m_vecAttachOffset(lua_State* L); // уст. m_vecAttachOffset (CPed) (x,y,z)
int get_ped_m_nAttachType(lua_State* L); // m_nAttachType (CPed)
int set_ped_m_nAttachType(lua_State* L); // уст. m_nAttachType (CPed)
int get_ped_m_fAttachRot(lua_State* L); // m_fAttachRot (CPed)
int set_ped_m_fAttachRot(lua_State* L); // уст. m_fAttachRot (CPed)
int get_ped_m_nAttachWepAmmo(lua_State* L); // m_nAttachWepAmmo (CPed)
int set_ped_m_nAttachWepAmmo(lua_State* L); // уст. m_nAttachWepAmmo (CPed)
int get_ped_m_nThreatFlags(lua_State* L); // m_nThreatFlags (CPed)
int set_ped_m_nThreatFlags(lua_State* L); // уст. m_nThreatFlags (CPed)
int get_ped_m_nThreatCheck(lua_State* L); // m_nThreatCheck (CPed)
int set_ped_m_nThreatCheck(lua_State* L); // уст. m_nThreatCheck (CPed)
int get_ped_m_nLastThreatCheck(lua_State* L); // m_nLastThreatCheck (CPed)
int set_ped_m_nLastThreatCheck(lua_State* L); // уст. m_nLastThreatCheck (CPed)
int get_ped_m_nSayType(lua_State* L); // m_nSayType (CPed)
int set_ped_m_nSayType(lua_State* L); // уст. m_nSayType (CPed)
int get_ped_m_nSayTimer(lua_State* L); // m_nSayTimer (CPed)
int set_ped_m_nSayTimer(lua_State* L); // уст. m_nSayTimer (CPed)
int get_ped_m_nTalkTimerLast(lua_State* L); // m_nTalkTimerLast (CPed)
int set_ped_m_nTalkTimerLast(lua_State* L); // уст. m_nTalkTimerLast (CPed)
int get_ped_m_nTalkTimer(lua_State* L); // m_nTalkTimer (CPed)
int set_ped_m_nTalkTimer(lua_State* L); // уст. m_nTalkTimer (CPed)
int get_ped_m_wTalkTypeLast(lua_State* L); // m_wTalkTypeLast (CPed)
int set_ped_m_wTalkTypeLast(lua_State* L); // уст. m_wTalkTypeLast (CPed)
int get_ped_m_wTalkType(lua_State* L); // m_wTalkType (CPed)
int set_ped_m_wTalkType(lua_State* L); // уст. m_wTalkType (CPed)
int get_ped_m_bCanPedTalk(lua_State* L); // m_bCanPedTalk (CPed)
int set_ped_m_bCanPedTalk(lua_State* L); // уст. m_bCanPedTalk (CPed)
int get_ped_m_nPedLastComment(lua_State* L); // m_nPedLastComment (CPed)
int set_ped_m_nPedLastComment(lua_State* L); // уст. m_nPedLastComment (CPed)
int get_ped_m_vecSeekPosEx(lua_State* L); // m_vecSeekPosEx (CPed) (x,y,z)
int set_ped_m_vecSeekPosEx(lua_State* L); // уст. m_vecSeekPosEx (CPed) (x,y,z)
int get_ped_m_fSeekExAngle(lua_State* L); // m_fSeekExAngle (CPed)
int set_ped_m_fSeekExAngle(lua_State* L); // уст. m_fSeekExAngle (CPed)
int get_obj_m_nObjectType(lua_State* L); // m_nObjectType (CObject)
int set_obj_m_nObjectType(lua_State* L); // уст. m_nObjectType (CObject)
int get_obj_m_nBonusValue(lua_State* L); // m_nBonusValue (CObject)
int set_obj_m_nBonusValue(lua_State* L); // уст. m_nBonusValue (CObject)
int get_obj_m_wCostValue(lua_State* L); // m_wCostValue (CObject)
int set_obj_m_wCostValue(lua_State* L); // уст. m_wCostValue (CObject)
int get_obj_m_fDamageMultiplier(lua_State* L); // m_fDamageMultiplier (CObject)
int set_obj_m_fDamageMultiplier(lua_State* L); // уст. m_fDamageMultiplier (CObject)
int get_obj_m_bCameraAvoids(lua_State* L); // m_bCameraAvoids (CObject)
int set_obj_m_bCameraAvoids(lua_State* L); // уст. m_bCameraAvoids (CObject)
int get_obj_m_wRefModelId(lua_State* L); // m_wRefModelId (CObject)
int set_obj_m_wRefModelId(lua_State* L); // уст. m_wRefModelId (CObject)
int get_car_m_nAudioEntityId(lua_State* L); // m_nAudioEntityId (CPhysical)
int set_car_m_nAudioEntityId(lua_State* L); // уст. m_nAudioEntityId (CPhysical)
int get_car_m_vecMoveSpeed(lua_State* L); // m_vecMoveSpeed (CPhysical) (x,y,z)
int set_car_m_vecMoveSpeed(lua_State* L); // уст. m_vecMoveSpeed (CPhysical) (x,y,z)
int get_car_m_vecTurnSpeed(lua_State* L); // m_vecTurnSpeed (CPhysical) (x,y,z)
int set_car_m_vecTurnSpeed(lua_State* L); // уст. m_vecTurnSpeed (CPhysical) (x,y,z)
int get_car_m_vecFrictionMoveForce(lua_State* L); // m_vecFrictionMoveForce (CPhysical) (x,y,z)
int set_car_m_vecFrictionMoveForce(lua_State* L); // уст. m_vecFrictionMoveForce (CPhysical) (x,y,z)
int get_car_m_vecFrictionTurnForce(lua_State* L); // m_vecFrictionTurnForce (CPhysical) (x,y,z)
int set_car_m_vecFrictionTurnForce(lua_State* L); // уст. m_vecFrictionTurnForce (CPhysical) (x,y,z)
int get_car_m_vecForce(lua_State* L); // m_vecForce (CPhysical) (x,y,z)
int set_car_m_vecForce(lua_State* L); // уст. m_vecForce (CPhysical) (x,y,z)
int get_car_m_vecTorque(lua_State* L); // m_vecTorque (CPhysical) (x,y,z)
int set_car_m_vecTorque(lua_State* L); // уст. m_vecTorque (CPhysical) (x,y,z)
int get_car_m_fMass(lua_State* L); // m_fMass (CPhysical)
int set_car_m_fMass(lua_State* L); // уст. m_fMass (CPhysical)
int get_car_m_fTurnMass(lua_State* L); // m_fTurnMass (CPhysical)
int set_car_m_fTurnMass(lua_State* L); // уст. m_fTurnMass (CPhysical)
int get_car_m_fVelocityFrequency(lua_State* L); // m_fVelocityFrequency (CPhysical)
int set_car_m_fVelocityFrequency(lua_State* L); // уст. m_fVelocityFrequency (CPhysical)
int get_car_m_fAirResistance(lua_State* L); // m_fAirResistance (CPhysical)
int set_car_m_fAirResistance(lua_State* L); // уст. m_fAirResistance (CPhysical)
int get_car_m_fElasticity(lua_State* L); // m_fElasticity (CPhysical)
int set_car_m_fElasticity(lua_State* L); // уст. m_fElasticity (CPhysical)
int get_car_m_fBuoyancyConstant(lua_State* L); // m_fBuoyancyConstant (CPhysical)
int set_car_m_fBuoyancyConstant(lua_State* L); // уст. m_fBuoyancyConstant (CPhysical)
int get_car_m_vecCentreOfMass(lua_State* L); // m_vecCentreOfMass (CPhysical) (x,y,z)
int set_car_m_vecCentreOfMass(lua_State* L); // уст. m_vecCentreOfMass (CPhysical) (x,y,z)
int get_car_m_nNumCollisionRecords(lua_State* L); // m_nNumCollisionRecords (CPhysical)
int set_car_m_nNumCollisionRecords(lua_State* L); // уст. m_nNumCollisionRecords (CPhysical)
int get_car_m_fTotSpeed(lua_State* L); // m_fTotSpeed (CPhysical)
int set_car_m_fTotSpeed(lua_State* L); // уст. m_fTotSpeed (CPhysical)
int get_car_m_fCollisionPower(lua_State* L); // m_fCollisionPower (CPhysical)
int set_car_m_fCollisionPower(lua_State* L); // уст. m_fCollisionPower (CPhysical)
int get_car_m_vecCollisionPower(lua_State* L); // m_vecCollisionPower (CPhysical) (x,y,z)
int set_car_m_vecCollisionPower(lua_State* L); // уст. m_vecCollisionPower (CPhysical) (x,y,z)
int get_car_m_wComponentCol(lua_State* L); // m_wComponentCol (CPhysical)
int set_car_m_wComponentCol(lua_State* L); // уст. m_wComponentCol (CPhysical)
int get_car_m_nMoveFlags(lua_State* L); // m_nMoveFlags (CPhysical)
int set_car_m_nMoveFlags(lua_State* L); // уст. m_nMoveFlags (CPhysical)
int get_car_m_nCollFlags(lua_State* L); // m_nCollFlags (CPhysical)
int set_car_m_nCollFlags(lua_State* L); // уст. m_nCollFlags (CPhysical)
int get_car_m_nLastCollType(lua_State* L); // m_nLastCollType (CPhysical)
int set_car_m_nLastCollType(lua_State* L); // уст. m_nLastCollType (CPhysical)
int get_car_m_nZoneLevel(lua_State* L); // m_nZoneLevel (CPhysical)
int set_car_m_nZoneLevel(lua_State* L); // уст. m_nZoneLevel (CPhysical)
int get_car_fNegSpeed(lua_State* L); // fNegSpeed (CAutomobile)
int set_car_fNegSpeed(lua_State* L); // уст. fNegSpeed (CAutomobile)
int get_car_fAIGripMultiplier(lua_State* L); // fAIGripMultiplier (CAutomobile)
int set_car_fAIGripMultiplier(lua_State* L); // уст. fAIGripMultiplier (CAutomobile)
int get_car_fSpecialWepRotH(lua_State* L); // fSpecialWepRotH (CAutomobile)
int set_car_fSpecialWepRotH(lua_State* L); // уст. fSpecialWepRotH (CAutomobile)
int get_car_fSpecialWepRotV(lua_State* L); // fSpecialWepRotV (CAutomobile)
int set_car_fSpecialWepRotV(lua_State* L); // уст. fSpecialWepRotV (CAutomobile)
int get_car_fSpecialSteering(lua_State* L); // fSpecialSteering (CAutomobile)
int set_car_fSpecialSteering(lua_State* L); // уст. fSpecialSteering (CAutomobile)
int get_car_fSpecialMoveState(lua_State* L); // fSpecialMoveState (CAutomobile)
int set_car_fSpecialMoveState(lua_State* L); // уст. fSpecialMoveState (CAutomobile)
int get_car_fSkidMarkDensity(lua_State* L); // fSkidMarkDensity (CAutomobile)
int set_car_fSkidMarkDensity(lua_State* L); // уст. fSkidMarkDensity (CAutomobile)
int get_car_m_fPropRotation(lua_State* L); // m_fPropRotation (CBoat)
int set_car_m_fPropRotation(lua_State* L); // уст. m_fPropRotation (CBoat)
int get_car_m_fPropSpeed(lua_State* L); // m_fPropSpeed (CBoat)
int set_car_m_fPropSpeed(lua_State* L); // уст. m_fPropSpeed (CBoat)
int get_car_m_fForcedZRotation(lua_State* L); // m_fForcedZRotation (CBoat)
int set_car_m_fForcedZRotation(lua_State* L); // уст. m_fForcedZRotation (CBoat)
int get_car_m_nAttackPlayerTime(lua_State* L); // m_nAttackPlayerTime (CBoat)
int set_car_m_nAttackPlayerTime(lua_State* L); // уст. m_nAttackPlayerTime (CBoat)
int get_car_m_fBurningTimer(lua_State* L); // m_fBurningTimer (CBoat)
int set_car_m_fBurningTimer(lua_State* L); // уст. m_fBurningTimer (CBoat)
int get_car_m_fBoatGasPedal(lua_State* L); // m_fBoatGasPedal (CBoat)
int set_car_m_fBoatGasPedal(lua_State* L); // уст. m_fBoatGasPedal (CBoat)
int get_car_m_fBoatBrakePedal(lua_State* L); // m_fBoatBrakePedal (CBoat)
int set_car_m_fBoatBrakePedal(lua_State* L); // уст. m_fBoatBrakePedal (CBoat)
int get_car_m_fBoatSteeringLeftRight(lua_State* L); // m_fBoatSteeringLeftRight (CBoat)
int set_car_m_fBoatSteeringLeftRight(lua_State* L); // уст. m_fBoatSteeringLeftRight (CBoat)
int get_car_m_nPadNumber(lua_State* L); // m_nPadNumber (CBoat)
int set_car_m_nPadNumber(lua_State* L); // уст. m_nPadNumber (CBoat)
int get_car_m_nNumWaterTrailPoints(lua_State* L); // m_nNumWaterTrailPoints (CBoat)
int set_car_m_nNumWaterTrailPoints(lua_State* L); // уст. m_nNumWaterTrailPoints (CBoat)
int get_car_nBikeAnimGroup(lua_State* L); // nBikeAnimGroup (CBike)
int set_car_nBikeAnimGroup(lua_State* L); // уст. nBikeAnimGroup (CBike)
int get_car_m_nDamageFlags(lua_State* L); // m_nDamageFlags (CBike)
int set_car_m_nDamageFlags(lua_State* L); // уст. m_nDamageFlags (CBike)
int get_car_m_fSearchLightX(lua_State* L); // m_fSearchLightX (CHeli)
int set_car_m_fSearchLightX(lua_State* L); // уст. m_fSearchLightX (CHeli)
int get_car_m_fSearchLightY(lua_State* L); // m_fSearchLightY (CHeli)
int set_car_m_fSearchLightY(lua_State* L); // уст. m_fSearchLightY (CHeli)
int get_car_m_nExplosionTime(lua_State* L); // m_nExplosionTime (CHeli)
int set_car_m_nExplosionTime(lua_State* L); // уст. m_nExplosionTime (CHeli)
int get_car_m_fRotationX(lua_State* L); // m_fRotationX (CHeli)
int set_car_m_fRotationX(lua_State* L); // уст. m_fRotationX (CHeli)
int get_car_m_fHeliForcedZ(lua_State* L); // m_fHeliForcedZ (CHeli)
int set_car_m_fHeliForcedZ(lua_State* L); // уст. m_fHeliForcedZ (CHeli)
int get_car_m_fSearchLightIntensity(lua_State* L); // m_fSearchLightIntensity (CHeli)
int set_car_m_fSearchLightIntensity(lua_State* L); // уст. m_fSearchLightIntensity (CHeli)
int get_car_m_nDamagePoints(lua_State* L); // m_nDamagePoints (CHeli)
int set_car_m_nDamagePoints(lua_State* L); // уст. m_nDamagePoints (CHeli)
int get_car_m_fRotorAngle(lua_State* L); // m_fRotorAngle (CHeli)
int set_car_m_fRotorAngle(lua_State* L); // уст. m_fRotorAngle (CHeli)
int get_wanted_m_nLastTimeWantedDecreased(lua_State* L); // последнее снижение розыска
int get_wanted_m_nLastTimeWantedLevelChanged(lua_State* L); // последняя смена уровня розыска
int get_wanted_m_dwTimeOfParole(lua_State* L); // таймер условного освобождения
int set_wanted_m_dwTimeOfParole(lua_State* L); // уст. таймер условного освобождения
int get_wanted_m_fMultiplier(lua_State* L); // множитель розыска
int set_wanted_m_fMultiplier(lua_State* L); // уст. множитель розыска
int get_wanted_m_nCopsInPursuit(lua_State* L); // копов в погоне
int get_wanted_m_nMaxCopsInPursuit(lua_State* L); // макс. копов в погоне
int set_wanted_m_nMaxCopsInPursuit(lua_State* L); // уст. макс. копов в погоне
int get_wanted_m_nMaxCopCarsInPursuit(lua_State* L); // макс. машин копов в погоне
int set_wanted_m_nMaxCopCarsInPursuit(lua_State* L); // уст. макс. машин копов в погоне
int get_wanted_m_nCopsBeatingSuspect(lua_State* L); // копов избивают подозреваемого
int choose_gang_ped_model(lua_State* L); // выбрать модель педа банды
int set_gang_attack_player_with_cops(lua_State* L); // банда атакует игрока с копами
int init_gangs(lua_State* L); // инициализировать банды
int register_motion_streak(lua_State* L); // размытие-полоса (id, левая/правая точки, rgb)
int init_trains(lua_State* L); // инициализировать поезда
int update_trains(lua_State* L); // обновить поезда
int shutdown_trains(lua_State* L); // завершить поезда
int Ped_in_targetting(lua_State* L); // Макрос пед на прицеле.
int Get_model_and_type_ped(lua_State* L); // макрос получить модель и тип педа.

int get_model_ped(lua_State* L); // получить модель педа.
int get_type_ped(lua_State* L); // получить тип педа.
int ped_clear_investigate_event(lua_State* L); // пед уходит, опустить педа.
int ped_clear_objective(lua_State* L); // снять задачи с педа.

int setbomb_in_car(lua_State* L); // установить бомбу в авто.
int getbomb_in_car(lua_State* L); // получить тип бомбы в авто.
int activetbomb(lua_State* L); // активировать бомбу в авто.
int is_ped_in_this_car(lua_State* L); // игрок в этом авто?

int ishorncar(lua_State* L); // проверить гудит ли авто.
int getcarfirstcolor(lua_State* L); // получить первый цвет авто.
int getcarseconscolor(lua_State* L); // получить второй цвет авто.
int ped_char_spotted_player(lua_State* L); // если пед увидел игрока.

int set_timer_bomb_car(lua_State* L); // установить таймер на взрыв бомбы в авто.
int is_ped_objective(lua_State* L); // если пед выполнил задачу.
int set_time_scale(lua_State* L); // установить ход времени в игре.
int is_ped_in_air(lua_State* L); // если пед в воздухе.

int kill_peds_in_car(lua_State* L); // убить педов в авто.
int getstamina_ped(lua_State* L); // получить выносливость педа.
int setstamina_ped(lua_State* L); // установить выносливость педа.
int get_radio_car(lua_State* L); // получить текущую радиостанцию авто.

int is_ped_passenger_car(lua_State* L); // пед пассажир авто.
int is_ped_driver_car(lua_State* L); // пед пассажир авто.
int ped_in_turret_on_car(lua_State* L); // переместить педа в турель авто.
int ped_from_turret_on_car(lua_State* L); // убрать педа из турели авто.

int is_button_pressed(lua_State* L); // если спец клавиша нажата.
int getmoney_ped(lua_State* L); // получить кол-во деньги педа.
int setmoney_ped(lua_State* L); // установить кол-во деньги педа.
int addmoney_ped(lua_State* L); // добавить кол-во деньги педа.

int getgravity(lua_State* L); // получить значения гравитации.
int setgravity(lua_State* L); // установить значения гравитации.
int gravity_default(lua_State* L); // вернуть значения по умолчанию гравитации.
int create_shots_on_ped(lua_State* L); // создать выстрелы.

int get_wheel_status(lua_State* L); // получить состояния шины авто.
int create_shots_on_car(lua_State* L); // создать выстрелы.
int ped_save_pos_attack(lua_State* L); // пед сохраняет ли свою позицию при атаке.
int ped_rebuff(lua_State* L); // пед атакует любого, кто нападает на него.

int ped_search_threat(lua_State* L); // пед ищет угрозу.
int ped_clean_threat(lua_State* L); // очистить угрозы для педа.
int save_car_color(lua_State* L); // авто сохраняет цвет при перекраске.
int is_car_passenger_free(lua_State* L); // Проверяет, свободно ли место пассажира в авто.

int set_ped_bleeding(lua_State* L); // вкл\выкл крованные у педа.
int all_exit_from_car(lua_State* L); // все педы выходят из авто.
int is_ped_duck(lua_State* L); // Проверяет, пед присел.
int set_car_tank(lua_State* L); // установить способность танка детонировать авто при контакте.

int set_display_radar(lua_State* L); // показать скрывать радар.
int set_ped_in_car(lua_State* L); // установить педа в авто как водителя или пассажира.
int set_ped_traffic(lua_State* L); // установить плотноcть трафика педов.
int heli_to_fly(lua_State* L); // вертолет летит на координаты.

int set_ped_immunities(lua_State* L); // уст иммунитеты педу.
int set_car_immunities(lua_State* L); // уст иммунитеты авто.
int setcolourmarker(lua_State* L); // установить цвет маркера.

int set_camera_on_car(lua_State* L); // уст камеру на авто.
int ped_run_to_point(lua_State* L); // пед бежит к точке пешком.
int select_interiour(lua_State* L); // уcтановить интерьер.
int set_ped_stats_to(lua_State* L); // уст поведения педа.

int ped_maintain_position_when_attacked(lua_State* L); // пед удерживает позицию при атаке.
int set_timer_ped_attack(lua_State* L); // установить таймер атаки педа.
int set_cops_ignored(lua_State* L); // установить игнор копов.
int set_camera_near_clip(lua_State* L); // установить обрезку камеры.

int setpedcrouch(lua_State* L); // пед сел.
int is_ped_incar(lua_State* L);  // пед в авто или нет?
int delete_entity(lua_State* L); // удалить сущность сразу.
int clean_leader(lua_State* L); // перестать следовать за лидером.

int show_save_menu(lua_State* L); // показать меню сохранения.
int is_ped_shooting(lua_State* L); // пед стреляет?.
int set_ped_visible(lua_State* L); // уст невидимость педу.
int set_player_mod(lua_State* L); // уст настроения игрока.

int setped_damaged_player(lua_State* L); // педу может нанести ущерб только игрок.
int set_ped_reaction_threat(lua_State* L); // уст реакцию педа на ответную угрозу.
int obj_target(lua_State* L); // в объект можно целиться.
int clean_ped_wait(lua_State* L); // пед больше не ждет.

int set_ped_accuracy(lua_State* L); // уст меткость педу.
int textbox(lua_State* L); // вывести сообщение в textbox.
int remove_textbox(lua_State* L); // удалить сообщение на textbox.
int get_controller(lua_State* L); // получить контролер от игрока.

int create_pickup_clothes(lua_State* L); // создать пикап одежды.
int set_riot_intensity(lua_State* L); // уст уровень шума беспорядков.
int set_ped_walk_style(lua_State* L); // установить стиль ходьбы педа.
int check_car_resray(lua_State* L); // проверить авто игрока было перекрашена в гараже.

int set_car_range(lua_State* L); // установить множитель диапазона на угрозу для автомобиля.
int set_ped_range(lua_State* L); // установить множитель диапазона на угрозу для педа.
int getcarangle(lua_State* L); // получить угол авто.
int cleanarea(lua_State* L); //очистить арену.

int set_brakes_car(lua_State* L); //уст тормоза авто игрока.
int setmarker_brightness(lua_State* L); // уст яркость маркера.
int Createobj(lua_State* L); // макрос создать объект.
int setpednode_seek(lua_State* L); // пед игнорирорует пути педов.

int ispedscreen(lua_State* L); // пед виден.
int iscarscreen(lua_State* L); // авто видно.
int isobjscreen(lua_State* L); // объект виден.
int ispeddead(lua_State* L); // пед мертв.

int iscardead(lua_State* L); // авто уничтожено.
int ped_follow_ped(lua_State* L); // пед следует за педом.
int set_cars_damaged(lua_State* L); // Все авто повреждены.
int set_ped_targetted(lua_State* L); // запрет целиться в педа.

int set_ped_friend(lua_State* L); // уст дружественное отношения педа.
int set_ped_running(lua_State* L); // пед может бежать.
int set_ped_damaged_gang(lua_State* L); // уст педа уязвимым для членов банды.
int is_ped_damaged_weapon(lua_State* L); // пед получает от определенного вида оружие.

int is_car_damaged_weapon(lua_State* L); // авто получает от определенного вида оружие.
int isped_in_air(lua_State* L); // пед в воздухе.
int set_threat_for_ped_type(lua_State* L); // уст враждебность типа педа к другим типам педа.
int clean_threat_for_ped_type(lua_State* L); // Убрать враждебность типа педа к другим типам педа.

int create_phone(lua_State* L); // создать телефон на координатах.
int on_phone(lua_State* L); // вкл телефон.
int off_phone(lua_State* L); // выкл телефон.
int read_memory(lua_State* L); // читать адрес памяти.

int write_memory(lua_State* L); // записать адрес памяти.
int is_sphere_onscreen(lua_State* L); // видна точка на радаре.
int set_ped_can_shot_in_car(lua_State* L); // водитель авто может получить пули.
int iscarfire(lua_State* L); // авто горит?

int is_car_damage_by_car(lua_State* L); // Если авто получило урон от другого авто.
int show_text_gtx_table(lua_State* L); // вывести текст из таблицы.
int ped_road_off(lua_State* L); // выкл путь педов.
int ped_road_on(lua_State* L); // вкл путь педов.

int car_road_off(lua_State* L); // выкл путь авто.
int car_road_on(lua_State* L); // вкл путь авто.
int getcardamage(lua_State* L); // получить кол-во урона авто.
int randomfindobj(lua_State* L); // найти случайный объект в радиусе.

int findped_in_cord(lua_State* L); // найти педа в радиусе с координатами.
int findcar_in_cord(lua_State* L); // найти авто в радиусе с координатами.
int findobj_in_cord(lua_State* L); // найти объект в радиусе с координатами.
int findped_nearest_in_cord(lua_State* L); // найти ближайшего педа в радиусе с координатами.

int findcar_nearest_in_cord(lua_State* L); // найти ближайшее авто в радиусе с координатами.
int findobj_nearest_in_cord(lua_State* L); // найти ближайший объект в радиусе с координатами.
int getmodelindex(lua_State* L); // получить id модели.
int check_ped_in_cord(lua_State* L); // есть ли педа в радиусе с координатами.

int get_ped_in_cord(lua_State* L); // получить педа в радиусе с координатами.
int check_car_in_cord(lua_State* L); // есть ли авто в радиусе с координатами.
int get_car_in_cord(lua_State* L); // найти авто в радиусе с координатами.
int check_obj_in_cord(lua_State* L); // есть ли объект в радиусе с координатами.

int get_obj_in_cord(lua_State* L); // получить объект в радиусе с координатам.
int setobjangle(lua_State* L); // уст угол объекта.
int getcardimension(lua_State* L); // получить размер авто.

int carmoveforce(lua_State* L);  // двигать авто с силой.
int carturnforce(lua_State* L);  // повернуть авто с силой.
int car_turn_speed(lua_State* L); // повернуть авто с применением скорости.
int carturnfrictionforce(lua_State* L); // повернуть авто с силой трения.

int car_move_speed(lua_State* L); // двигать авто с применением скорости.
int car_turnforce_on_x(lua_State* L); // Повернуть авто по оси x.
int car_turnforce_on_y(lua_State* L); // Повернуть авто по оси y.
int car_turnforce_on_z(lua_State* L); // Повернуть авто по оси z.

int car_moveforce_on_x(lua_State* L); // Двигать авто по оси x.
int car_moveforce_on_y(lua_State* L); // Двигать авто по оси y.
int car_moveforce_on_z(lua_State* L); // Двигать авто по оси z.
int setcarrotate(lua_State* L);  // уст вращения авто.

int getcar_turn_on_x(lua_State* L); // Получить угол поворота авто по оси x.
int getcar_turn_on_y(lua_State* L); // Получить угол поворота авто по оси y.
int getcar_turn_on_z(lua_State* L); // Получить угол поворота авто по оси z.

int carmovespeed_on_x(lua_State* L); // двигать авто с скоростью по x.
int carmovespeed_on_y(lua_State* L); // двигать авто с скоростью по y.
int carmovespeed_on_z(lua_State* L); // двигать авто с скоростью по z.

int car_rotate_on_x(lua_State* L); // Поворот угла авто по оси x.
int car_rotate_on_y(lua_State* L); // Поворот угла авто по оси y.
int car_rotate_on_z(lua_State* L); // Поворот угла авто по оси z.

int car_setrotate_on_x(lua_State* L); // уст авто по оси x.
int car_setrotate_on_y(lua_State* L); // уст авто по оси y.
int car_setrotate_on_z(lua_State* L); // уст авто по оси z.

int getcar_resistance(lua_State* L); // Получить сопротивления авто трению.
int setcar_resistance(lua_State* L); // Уст силу сопротивления авто трению.
int getcar_turns_on_axes(lua_State* L); // получить углы авто поворота по осям.

int setcar_on_x(lua_State* L); // уст авто по оси x.
int setcar_on_y(lua_State* L); // уст авто по оси y.
int setcar_on_z(lua_State* L); // уст авто по оси z.

int car_rotate_turn_on_x(lua_State* L); // уст угол поворота авто по оси x.
int car_rotate_turn_on_y(lua_State* L); // уст угол поворота авто по оси y.
int car_rotate_turn_on_z(lua_State* L); // уст угол поворота авто по оси z.

int car_turn_on_x_with_speed(lua_State* L); //повeрнуть авто по оси x на угол со скорость.
int car_turn_on_y_with_speed(lua_State* L); //повeрнуть авто по оси y на угол со скорость.
int car_turn_on_z_with_speed(lua_State* L); //повeрнуть авто по оси z на угол со скорость.


int car_turn_on_x_with_delay(lua_State* L); //повeрнуть авто по оси x на угол со задержкой.
int car_turn_on_y_with_delay(lua_State* L); //повeрнуть авто по оси y на угол со задержкой.
int car_turn_on_z_with_delay(lua_State* L); //повeрнуть авто по оси z на угол со задержкой.
int set_car_mass(lua_State* L); // set_car_mass(car, 3000) - увеличить массу авто (значение как в handling.cfg, 3000 ~ грузовик).
int set_car_acceleration(lua_State* L); // set_car_acceleration(car, 0.2) - установить ускорение авто (0.1..10.0).
int set_car_max_speed(lua_State* L); // set_car_max_speed(car, 100) - установить макс. скорость авто.
int set_car_traction(lua_State* L); // set_car_traction(car, 0.9) - установить сцепление авто (0.8..1.0).

int create_rope_on_cords_with_swat(lua_State* L); // создать веревку на координатах с бойцом спецназа.
int create_rope_on_cords(lua_State* L); // создать веревку на координатах.
int heli_change_height(lua_State* L); // изменить высоту вертолета.

int set_path_to_module(lua_State* L);// уст путь к модулю.
int load_and_start_luascript(lua_State* L, char* luafile, string res); // загрузка и запуск скрипта.

int newthread(lua_State* L);// запуск функции в новом потоке.

void reversestack(lua_State* L); // реверс стека.
void showstack(lua_State* L);
void showstack1(lua_State* L);

int my_yield(lua_State* L) { return lua_yield(L, 0); }; //приостановить выполнение скрипта.

void writelog1(int x);
int timerstar();

int findplayer(lua_State* L) {//  найти томми.
	CPed* player = FindPlayerPed();// найти томми.
	lua_pushlightuserdata(L, player);// отправить в стек указатель на педа.
	return 1;
};

int cardrive(lua_State* L) {// авто едет в точку.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)
			&& LUA_TNUMBER == lua_type(L, 4)) {// значение число.

			const void* p1 = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p1);//  получить указатель на авто.
			float x = lua_tonumber(L, 2); float y = lua_tonumber(L, 3);
			float z = lua_tonumber(L, 4); CVector pos = { x, y, z };// вектор координат.
			Command<COMMAND_CAR_GOTO_COORDINATES>(car, pos.x, pos.y, pos.z);// авто едет на координаты.
			return 0;
		}// int

		else { throw "bad argument in function cardrive"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int wait(lua_State* L) {
	static int delay = 0;
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// значение число.
			int time = lua_tointeger(L, 1); // время задержки.
			this_thread::sleep_for(chrono::milliseconds(time));
			return 0;
		}// int
		if (LUA_TSTRING == lua_type(L, 1) || LUA_TBOOLEAN == lua_type(L, 1)) {
			throw "bad argument in function wait";
		}
		if (LUA_TBOOLEAN == lua_type(L, 1)) {
			throw "bad argument in function wait";
		}
		else {
			this_thread::sleep_for(chrono::milliseconds(1)); return 0;
			/*	delay = iters;
				while (true)
				{
				if (iters- delay >1){
					delay = 0;
					break;
				}
				}*/
		}
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int setpedhealth(lua_State* L) {// установить здоровье педу.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			if (LUA_TNUMBER == lua_type(L, 2)) {
				const void* p = lua_topointer(L, 1);

				CPed* ped = findpedinpool(p);// получить указатель на педа.

				float health = lua_tonumber(L, 2);// если число.

				health += 0.99f; ped->m_fHealth = health;
				return 0;
			}// установить здоровье педа.
			else { throw "bad argument in function setpedhealth option health"; }
		}
		else { throw "bad argument in function setpedhealth option of the player"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int setarmour(lua_State* L) {// установить броню педу.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			if (LUA_TNUMBER == lua_type(L, 2)) {
				const void* p = lua_topointer(L, 1);

				CPed* ped = findpedinpool(p);// получить указатель на педа.
				float armour = lua_tonumber(L, 2);
				armour += 0.10f; ped->m_fArmour = armour; return 0;
			}// установить броню педа.
			else { throw "bad argument in function setarmour option health"; }
		}
		else { throw "bad argument in function setarmour option of the player"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int getpedarmour(lua_State* L) {// получить кол-во брони педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			float armour = ped->m_fArmour;
			lua_pushinteger(L, armour);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function getpedarmour option of the player"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int getpedhealth(lua_State* L) { // получить кол-во здоровья педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);

			CPed* ped = findpedinpool(p);// получить указатель на педа.
			if (ped == NULL) { lua_pushinteger(L, 0); return 1; }
			int health = ped->m_fHealth; // получить кол-во здоровья педа.
			lua_pushinteger(L, health);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function getpedhealth option of the player"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int getcarhealth(lua_State* L) { // получить кол-во здоровья авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.

			if (car == NULL) { lua_pushinteger(L, 0); return 1; }
			int health = car->m_fHealth; // получить кол-во здоровья авто.
			lua_pushinteger(L, health);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function getcarhealth option of the vehicle"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int setcarhealth(lua_State* L) {// установить здоровье авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			if (LUA_TNUMBER == lua_type(L, 2)) {// здоровье авто.
				const void* p = lua_topointer(L, 1);
				CVehicle* car = findcarinpool(p);//  получить указатель на авто.

				float health = lua_tonumber(L, 2);// если число.
				health += 0.99f; car->m_fHealth = health; return 0;
			}// установить здоровье авто.
			else { throw "bad argument in function setcarhealth option health"; }
		}
		else { throw "bad argument in function setcarhealth option of the vehicle"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int setcarangle(lua_State* L) {// установить угол авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			if (LUA_TNUMBER == lua_type(L, 2)) {

				const void* p = lua_topointer(L, 1);
				CVehicle* car = findcarinpool(p);//  получить указатель на авто.
				if (car == NULL) { writelog("setcarangle: car is NULL"); return 0; }

				float angle = lua_tonumber(L, 2);// угол авто.

				Command<COMMAND_SET_CAR_HEADING>(CPools::GetVehicleRef(car), angle);

				return 0;
			}
			else { throw "bad argument in function setcarangle option angle"; }
		}
		else { throw "bad argument in function setcarangle option of the vehicle"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int setpedangle(lua_State* L) {// установить угол педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на авто.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			if (ped == NULL) { writelog("setpedangle: ped is NULL"); return 0; }
			float angle = lua_tonumber(L, 2);// угол авто.
			CPed* player = FindPlayerPed();// найти игрока.
			if (ped == player) {
				Command<COMMAND_SET_PLAYER_HEADING>(CWorld::PlayerInFocus, angle);
				return 0;
			}
			else {
				Command<COMMAND_SET_CHAR_HEADING>(CPools::GetPedRef(ped), angle);
				return 0;
			}
		}
		else { throw "bad argument in function setcarangle option of the vehicle"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int setobjangle(lua_State* L) {// уст угол объекта.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2))  {// указатель на объект.
			const void* p = lua_topointer(L, 1);
			CObject* obj = findobjinpool(p);// получить указатель на педа.

			float angle = lua_tonumber(L, 2);// угол объекта.
			Command<COMMAND_SET_OBJECT_HEADING>(CPools::GetObjectRef(obj), angle);// уст угол объекта.
			return 0;
		}
		else { throw "bad argument in function setobjangle."; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int setdrivingstyle(lua_State* L) {// установить стиль езды авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			if (LUA_TNUMBER == lua_type(L, 2)) {

				const void* p = lua_topointer(L, 1);
				CVehicle* car = findcarinpool(p);//  получить указатель на авто.

				int style = lua_tointeger(L, 2);// если число.
				switch (style) {
				case 0: {car->m_autoPilot.m_nDrivingStyle = DRIVINGSTYLE_STOP_FOR_CARS;
					break; }// ОСТАНОВКА СТИЛЯ ВОЖДЕНИЯ ДЛЯ АВТОМОБИЛЕЙ;
				case 1: {car->m_autoPilot.m_nDrivingStyle = DRIVINGSTYLE_SLOW_DOWN_FOR_CARS;
					break; }// СТИЛЬ ВОЖДЕНИЯ ЗАМЕДЛИТЬ ДЛЯ АВТОМОБИЛЕЙ;,
				case 2: {car->m_autoPilot.m_nDrivingStyle = DRIVINGSTYLE_AVOID_CARS;
					break; }// СТИЛЬ ВОЖДЕНИЯ ИЗБЕГАЙТЕ АВТОМОБИЛЕЙ;,
				case 3: {car->m_autoPilot.m_nDrivingStyle = DRIVINGSTYLE_PLOUGH_THROUGH;
					break; }//	СТИЛЬ ВОЖДЕНИЯ ПЛУГ ЧЕРЕЗ;,
				case 4: {car->m_autoPilot.m_nDrivingStyle = DRIVINGSTYLE_STOP_FOR_CARS_IGNORE_LIGHTS;
					break; }//	СТИЛЬ ВОЖДЕНИЯ СТОП ДЛЯ АВТОМОБИЛЕЙ ИГНОРИРОВАТЬ ОГНИ;
				default: {}
				}
				return 0;
			}// установить стиль езды авто.
			else { throw "bad argument in function setdrivingstyle option style"; }
		}
		else { throw "bad argument in function setdrivingstyle option of the vehicle"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int setcaraction(lua_State* L) {// установить поведение авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			if (LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {

				const void* p = lua_topointer(L, 1);
				CVehicle* car = findcarinpool(p);//  получить указатель на авто.

				int style = lua_tointeger(L, 2);// действие авто.
				unsigned int t = lua_tointeger(L, 3);// время выполнение действия авто.
				unsigned int time = t * 10;

				for (int i = 0; i < CPools::ms_pVehiclePool->m_nSize; i++) {
					CVehicle* vehicle = CPools::ms_pVehiclePool->GetAt(i);
					if (vehicle == car) {

						if (style == 0) {
							Command<COMMAND_SET_CAR_TEMP_ACTION>(CPools::GetVehicleRef(vehicle), TEMPACT_NONE, time);
							return 0;
						}
						if (style == 1) {
							Command<COMMAND_SET_CAR_TEMP_ACTION>(CPools::GetVehicleRef(vehicle), TEMPACT_WAIT, time);
							return 0;
						}
						if (style == 2) {
							Command<COMMAND_SET_CAR_TEMP_ACTION>(CPools::GetVehicleRef(vehicle), TEMPACT_REVERSE, time);
							return 0;
						}
						if (style == 3) {
							Command<COMMAND_SET_CAR_TEMP_ACTION>(CPools::GetVehicleRef(vehicle), TEMPACT_HANDBRAKETURNLEFT, time);
							return 0;
						}
						if (style == 4) {
							Command<COMMAND_SET_CAR_TEMP_ACTION>(CPools::GetVehicleRef(vehicle), TEMPACT_HANDBRAKETURNRIGHT, time);
							return 0;
						}
						if (style == 5) {
							Command<COMMAND_SET_CAR_TEMP_ACTION>(CPools::GetVehicleRef(vehicle), TEMPACT_HANDBRAKESTRAIGHT, time);
							return 0;
						}
						if (style == 6) {//влево.
							Command<COMMAND_SET_CAR_TEMP_ACTION>(CPools::GetVehicleRef(vehicle), TEMPACT_TURNLEFT, time);
							return 0;
						}
						if (style == 7) {// вправо.
							Command<COMMAND_SET_CAR_TEMP_ACTION>(CPools::GetVehicleRef(vehicle), TEMPACT_TURNRIGHT, time);
							return 0;
						}
						if (style == 8) {// вперед.
							Command<COMMAND_SET_CAR_TEMP_ACTION>(CPools::GetVehicleRef(vehicle), TEMPACT_GOFORWARD, time);
							return 0;
						}
						if (style == 9) {
							Command<COMMAND_SET_CAR_TEMP_ACTION>(CPools::GetVehicleRef(vehicle), TEMPACT_SWERVELEFT, time);
							return 0;
						}
						if (style == 10) {
							Command<COMMAND_SET_CAR_TEMP_ACTION>(CPools::GetVehicleRef(vehicle), TEMPACT_SWERVERIGHT, time);
							return 0;
						}
					}
				}
			}// установить действие авто.
			else { throw "bad argument in function setcaraction"; }
		}
		else { throw "bad argument in function setcaraction option of the vehicle"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int setcarspeed(lua_State* L) {// установить скорость авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			if (LUA_TNUMBER == lua_type(L, 2)) {

				const void* p = lua_topointer(L, 1);
				CVehicle* car = findcarinpool(p);//  получить указатель на авто.

				float speed = lua_tonumber(L, 2);// переменная хранить скорость авто.
				Command<COMMAND_SET_CAR_CRUISE_SPEED>(CPools::GetVehicleRef(car), speed);
				return 0;
			}
			else { throw "bad argument in function setcarspeed option speed"; }
		}
		else { throw "bad argument in function setcarspeed option of the vehicle"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int setcartask(lua_State* L) {// установить задачу авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			if (LUA_TNUMBER == lua_type(L, 2)) {
				const void* p = lua_topointer(L, 1);

				CVehicle* car = findcarinpool(p);//  получить указатель на авто.
/*0 - авто игрока(управляется на расстоянии через жопу/альтернативное управление)
1 - авто бота(игрок не может зайти/выйти из машины)
2 - езда по трафику
3- машина тормозит
5 - авто превращается в физический обьект, как например взорванное*/
				int task = lua_tointeger(L, 2);// задача авто.
				if (task == 0) {
					car->m_autoPilot.m_nCarMission = MISSION_NONE;
					return 0;
				}
				if (task == 1) {
					car->m_autoPilot.m_nCarMission = MISSION_CRUISE;
					return 0;
				}
				if (task == 2) {
					car->m_autoPilot.m_nCarMission = MISSION_RAMPLAYER_FARAWAY;
					return 0;
				}
				if (task == 3) {
					car->m_autoPilot.m_nCarMission = MISSION_RAMPLAYER_CLOSE;//ехать за педом
					return 0;
				}
				if (task == 4) {
					car->m_autoPilot.m_nCarMission = MISSION_BLOCKPLAYER_FARAWAY;
					return 0;
				}
				if (task == 5) {
					car->m_autoPilot.m_nCarMission = MISSION_BLOCKPLAYER_CLOSE;// подъехать к педу.
					return 0;
				}
				if (task == 6) {//влево.
					car->m_autoPilot.m_nCarMission = MISSION_BLOCKPLAYER_HANDBRAKESTOP;
					return 0;
				}
				if (task == 7) {// вправо.
					car->m_autoPilot.m_nCarMission = MISSION_WAITFORDELETION;
					return 0;
				}
				if (task == 8) {// вперед.
					car->m_autoPilot.m_nCarMission = MISSION_GOTOCOORDS;
					return 0;
				}
				if (task == 9) {
					car->m_autoPilot.m_nCarMission = MISSION_GOTOCOORDS_STRAIGHT;
					return 0;
				}
				if (task == 10) {
					car->m_autoPilot.m_nCarMission = MISSION_EMERGENCYVEHICLE_STOP;
					return 0;
				}
				if (task == 11) {
					car->m_autoPilot.m_nCarMission = MISSION_STOP_FOREVER;
					return 0;
				}
				if (task == 12) {
					car->m_autoPilot.m_nCarMission = MISSION_GOTOCOORDS_ACCURATE;
					return 0;
				}
				if (task == 13) {
					car->m_autoPilot.m_nCarMission = MISSION_GOTO_COORDS_STRAIGHT_ACCURATE;
					return 0;
				}
				if (task == 14) {
					car->m_autoPilot.m_nCarMission = MISSION_GOTOCOORDS_ASTHECROWSWIMS;
					return 0;
				}
				if (task == 15) {
					car->m_autoPilot.m_nCarMission = MISSION_RAMCAR_FARAWAY;
					return 0;
				}
				if (task == 16) {
					car->m_autoPilot.m_nCarMission = MISSION_RAMCAR_CLOSE;
					return 0;
				}
				if (task == 17) {
					car->m_autoPilot.m_nCarMission = MISSION_BLOCKCAR_FARAWAY;
					return 0;
				}
				if (task == 18) {
					car->m_autoPilot.m_nCarMission = MISSION_BLOCKCAR_CLOSE;
					return 0;
				}
				if (task == 19) {
					car->m_autoPilot.m_nCarMission = MISSION_BLOCKCAR_HANDBRAKESTOP;
					return 0;
				}
				if (task == 20) {
					car->m_autoPilot.m_nCarMission = MISSION_HELI_FLYTOCOORS;
					return 0;
				}
				if (task == 21) {
					car->m_autoPilot.m_nCarMission = MISSION_ATTACKPLAYER;
					return 0;
				}
				if (task == 22) {
					car->m_autoPilot.m_nCarMission = MISSION_PLANE_FLYTOCOORS;
					return 0;
				}
				if (task == 23) {
					car->m_autoPilot.m_nCarMission = MISSION_HELI_LAND;
					return 0;
				}
				if (task == 24) {
					car->m_autoPilot.m_nCarMission = MISSION_SLOWLY_DRIVE_TOWARDS_PLAYER_1;
					return 0;
				}
				if (task == 25) {
					car->m_autoPilot.m_nCarMission = MISSION_SLOWLY_DRIVE_TOWARDS_PLAYER_2;
					return 0;
				}
				if (task == 26) {
					car->m_autoPilot.m_nCarMission = MISSION_BLOCKPLAYER_FORWARDANDBACK;
					return 0;
				}
				return 0;
			}
			else { throw "bad argument in function setcartask option task"; }
		}
		else { throw "bad argument in function setcartask option of the vehicle"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int keypress(lua_State* L) {// проверка нажата ли клавиша?
	try {
		if (LUA_TSTRING == lua_type(L, 1)) {// значение строка.
			const char* namekey = lua_tostring(L, 1);
			int key = find_in_map(numbers_keys, namekey);// номер клавиши.

			if (GetAsyncKeyState(key) == -32767) {
				lua_pushboolean(L, true);// клавиша нажата.
				return 1;
			}
			else {
				lua_pushboolean(L, false);
				return 1;
			}
		}
		else { throw "bad argument in function keypress"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int lockstatus(lua_State* L) {// статус двери авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на авто.

			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.
			/*
			0	CARLOCK_NONE
			1	CARLOCK_UNLOCKED разблокированная машина
			2	CARLOCK_LOCKED	запертая машина - свойства включают двери не могут упасть, никто не может войти, но может выйти, любой может войти, если дверь оставлена ​​широко открытой или упала
			3	CARLOCK_LOCKOUT_PLAYER_ONLY	заблокирован для игрока, не заблокирован для NPC
			4	CARLOCK_LOCKED_PLAYER_INSIDE заблокирован, игрок застрял в машине
			5	CARLOCK_LOCKED_INITIALLY заблокирован, когда не в машине, разблокирован, когда в машине
			6	CARLOCK_FORCE_SHUT_DOORS анимация никогда не оставит дверь открытой, всегда закрывает дверь
			7	CARLOCK_LOCKED_BUT_CAN_BE_DAMAGED заперт, но двери могут упасть
			*/
			int status = lua_tointeger(L, 2);// статус двери авто.
			car->m_eDoorLock = (eDoorLock)status;
			return 0;
		}
		else { throw "bad argument in function lockstatus"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int givemoney(lua_State* L) {// дать денег игроку.
 	try {
 		if (LUA_TNUMBER == lua_type(L, 1)) {//кол-во денег.
 			int money = lua_tointeger(L, 1);
 			cpp_tracef("givemoney +%d", money);
 			CWorld::Players[CWorld::PlayerInFocus].m_nMoney += money;// дать денег
 		}
 		else { throw "bad argument in function givemoney"; }
 	}
 	catch (const char* x) { writelog(x); }// записать ошибку в файл.
 	return 0;
 };

int getpedcoordes(lua_State* L) {// получить координаты педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.

			lua_pushnumber(L, ped->GetPosition().x);// отправить в стек.
			lua_pushnumber(L, ped->GetPosition().y);// отправить в стек.
			lua_pushnumber(L, ped->GetPosition().z);// отправить в стек.
			return 3;
		}   // получить координаты педа.

		else { throw "bad argument in function getpedcoordes option of the player"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int getcarcoordes(lua_State* L) {// получить координаты авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.

			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.

			lua_pushnumber(L, car->GetPosition().x);// отправить в стек.
			lua_pushnumber(L, car->GetPosition().y);// отправить в стек.
			lua_pushnumber(L, car->GetPosition().z);// отправить в стек.
			return 3;
		}   // получить координаты авто.

		else { throw "bad argument in function getcarcoordes"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int printmessage(lua_State* L) {// аргументы текст и и время вывода на экран.
	try {
		if (LUA_TSTRING == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {//строка.
			const char* c = lua_tostring(L, 1);// строка.
			int time = lua_tointeger(L, 2);// время вывода текста.
			int style = lua_tointeger(L, 3);// стиль вывода текста.
			wchar_t* str = getwchat(c);
			CMessages::AddMessageJumpQ(str, time, style);// вывести сообщение на экран.
			return 0;
		}
		else { throw "bad argument in function printmessage"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int randomfindped(lua_State* L) {// найти педа в радиусе.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {

			const void* p1 = lua_topointer(L, 1);

			CPed* p = findpedinpool(p1);//  получить указатель на педа.
			float radius = lua_tonumber(L, 2);	CVehicle* v = NULL;

			if (p->m_bInVehicle && p->m_pVehicle != NULL) {// в авто пед?
				CVehicle* v = p->m_pVehicle;}// получить указатель на хенлд авто в котором сидит томии.

			for (auto car : CPools::ms_pVehiclePool) {
				if (car != v && CVector::Distance(car->GetPosition(), p->GetPosition()) < radius && car->m_fHealth > 50) {
					car->CanPedExitCar(true);
					if (CPed * p1 = car->m_pDriver) {
						if (p1 != NULL && p1 != p) {
							lua_pushboolean(L, true); lua_pushlightuserdata(L, p1);// отправить в стек и получить из стека можно
							return 2;
						}
					}
				}
			}
			for (auto ped : CPools::ms_pPedPool) {
				if (ped != p && CVector::Distance(ped->GetPosition(), p->GetPosition()) < radius && ped->m_fHealth > 50) {
					lua_pushboolean(L, true); lua_pushlightuserdata(L, ped);// отправить в стек и получить из стека можно
					return 2;	} }//
			CPed* p2 = NULL; lua_pushboolean(L, false);
			lua_pushlightuserdata(L, p2);// отправить в стек и получить из стека можно
			return 2;
		}
		else { throw "bad argument in function randomfindped"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int randomfindcar(lua_State* L) {//Найти случайное авто в радиусе.
	try { if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {

			const void* p1 = lua_topointer(L, 1);
			CPed* p = findpedinpool(p1);//  получить указатель на авто.

			float radius = lua_tointeger(L, 2);// радиус.
			CVehicle* v = NULL;

			if (p->m_bInVehicle && p->m_pVehicle != NULL) {// в авто пед?
				CVehicle* v = p->m_pVehicle;	}// получить указатель на хенлд авто в котором сидит томии.
			for (auto car : CPools::ms_pVehiclePool) {
				if (car != v && CVector::Distance(car->GetPosition(), p->GetPosition()) < radius && car->m_fHealth > 50) {

					lua_pushboolean(L, true); lua_pushlightuserdata(L, car);// отправить в стек и получить из стека можно
					return 2;
				}
			};
			CVehicle* car = NULL;
			lua_pushboolean(L, false); lua_pushlightuserdata(L, car);// отправить в стек и получить из стека можно
			return 2;

		}
		else { throw "bad argument in function randomfindcar"; }
	}
	catch (const char* x) {	writelog(x);}// записать ошибку в файл.
};

int randomfindobj(lua_State* L) {// найти случайный объект в радиусе.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {

			const void* p1 = lua_topointer(L, 1);

			CPed* p = findpedinpool(p1);//  получить указатель на педа.
			float radius = lua_tonumber(L, 2);

			for (auto obj : CPools::ms_pObjectPool) {
				if (CVector::Distance(obj->GetPosition(), p->GetPosition()) < radius) {
					lua_pushboolean(L, true); lua_pushlightuserdata(L, obj);// отправить в стек и получить из стека можно
					return 2;	}		}//
		    CObject* obj1 = NULL; lua_pushboolean(L, false);
			lua_pushlightuserdata(L, obj1);// отправить в стек и получить из стека можно
			return 2;
		}
		else { throw "bad argument in function randomfindobj"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int findcar(lua_State* L) {//Найти авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {

			const void* p = lua_topointer(L, 1);
			CVehicle* v = findcarinpool(p);//  получить указатель на авто.
			for (auto car : CPools::ms_pVehiclePool) {
				if (car == v) {
					lua_pushlightuserdata(L, car);// отправить в стек и получить из стека можно
					return 1;
				}
			}
		}
		else { throw "bad argument in function findcar"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int findped(lua_State* L) {
	try {
		lua_newtable(L);
		luaL_newmetatable(L, "mt");
		for (auto ped : CPools::ms_pPedPool) {
			lua_pushlightuserdata(L, ped);
			lua_pushlightuserdata(L, ped);// отправить в стек и получить из стека можно
			lua_rawset(L, -3);
			luaL_setmetatable(L, "mt");
		}
		return 1;
	}

	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int incar(lua_State* L) {// пед в авто?
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			if (ped->m_bInVehicle && ped->m_pVehicle != NULL) {// в авто пед?
				CVehicle* v = ped->m_pVehicle;
				lua_pushboolean(L, true);
				lua_pushlightuserdata(L, v);// отправить в стек true и указатель на авто.
				return 2;
			}
			else {
				CVehicle* v = NULL;//если пед не в авто вернуть null;
				lua_pushboolean(L, false);
				lua_pushlightuserdata(L, v);// отправить в стек и получить из стека можно
				return 2;// получить указатель на хенлд авто в котором сидит пед.
			}
		}
		else { throw "bad argument in function incar"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int exitcar(lua_State* L) {// пед выходит из машины.
 	cpp_trace("exitcar: ВХОД");
 	try {
 		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
 			const void* p = lua_topointer(L, 1);
 			CPed* ped = findpedinpool(p);// получить указатель на педа.
 			if (ped == NULL) { cpp_trace("exitcar: пед НЕ найден в пуле, ВЫХОД"); return 0; }
 			cpp_tracef("exitcar: пед=%p m_bInVehicle=%d m_pVehicle=%p", ped, (int)ped->m_bInVehicle, (void*)ped->m_pVehicle);
 			if (!ped->m_bInVehicle || ped->m_pVehicle == NULL) { cpp_trace("exitcar: пед не в авто, ВЫХОД"); return 0; }
 			ped->SetObjective(OBJECTIVE_LEAVE_CAR);
 			cpp_trace("exitcar: SetObjective LEAVE_CAR OK");
 			return 0;
 		} // выйти из авто.
 		else { throw "bad argument in function exitcar"; }
 	}
 	catch (const char* x) { writelog(x); }// записать ошибку в файл.
 };

int loadmodel(lua_State* L) {//Загрузка моделей.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// значение число(модель).
			int model = lua_tointeger(L, 1);
			Command<COMMAND_REQUEST_MODEL>(model);
			return 0;
		}// int

		else { throw "bad argument in function loadmodel"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int releasemodel(lua_State* L) {// Удалить модель из памяти.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// значение id модели.
			int model = lua_tointeger(L, 1);
			Command<COMMAND_MARK_MODEL_AS_NO_LONGER_NEEDED>(model);
			return 0;
		}// int
		else { throw "bad argument in function releasemodel"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int availablemodel(lua_State* L) {// проверка на загруженность модели.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// значение id модели.
			int model = lua_tointeger(L, 1);
			bool ava = Command<COMMAND_HAS_MODEL_LOADED>(model);
			lua_pushboolean(L, ava);
			return 1;
		}// int

		else { throw "bad argument in function availablemodel"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int createcar(lua_State* L) {// создать авто на координатах.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)
			&& LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {// значение число.
			int model = lua_tointeger(L, 1);// модель авто.
			float x = lua_tonumber(L, 2); float y = lua_tonumber(L, 3);
			float z = lua_tonumber(L, 4); CVector pos = { x, y, z };
			CVehicle* vehicle = NULL;
			Command<COMMAND_CREATE_CAR>(model, pos.x, pos.y, pos.z, &vehicle);
			mapcars.emplace(vehicle, L);// добавить в map для авто.
			lua_pushlightuserdata(L, vehicle);// отправить в стек указатель на авто.
			return 1;
		}// int

		else { throw "bad argument in function createcar"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int createobj(lua_State* L) {// создать объект.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)
			&& LUA_TNUMBER == lua_type(L, 4)) {// значение число.
			int model = lua_tointeger(L, 1);// модель авто.
			float x = lua_tonumber(L, 2); float y = lua_tonumber(L, 3);
			float z = lua_tonumber(L, 4); CVector pos = { x, y, z };
			CObject* obj = NULL;
			Command<COMMAND_CREATE_OBJECT>(model, pos.x, pos.y, pos.z, &obj);
			mapobjs.emplace(obj, L);// добавить в map для авто.
			lua_pushlightuserdata(L, obj);// отправить в стек указатель на объект.
			return 1;
		}// int

		else { throw "bad argument in function createobj"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int create_marker_actor(lua_State* L) {//создать маркер над педом.
	int marker;// храним id маркера.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			if (ped == NULL) return 0;
			Command<COMMAND_ADD_BLIP_FOR_CHAR>(CPools::GetPedRef(ped), &marker);
			markeron.emplace(marker, L);// добавить в map для маркеров.
			lua_pushinteger(L, marker);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function create_marker_actor"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int create_marker_car(lua_State* L) {//создать маркер над авто.
	int marker;// храним id маркера.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.

			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.
			Command<COMMAND_ADD_BLIP_FOR_CAR>(CPools::GetVehicleRef(car), &marker);

			markeron.emplace(marker, L);// добавить в map для маркеров.
			lua_pushinteger(L, marker);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function create_marker_car"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int removemarker(lua_State* L) {// удалить маркер.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// значение число.
			int marker = lua_tointeger(L, 1);// получить id маркера.
			Command<COMMAND_REMOVE_BLIP>(marker);
			auto it = markeron.find(marker);
			if (it != markeron.end() && L == it->second) {
				markeron.erase(it);
			}
			return 0;
		}
		else { throw "bad argument in function removemarker option of the player"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_sprint_to_point(lua_State* L) {// пед делает спринт к точке.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {// указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);//  получить указатель на педа.
			if (ped == NULL) return 0;

			float x = lua_tonumber(L, 2); float y = lua_tonumber(L, 3);
			float z = lua_tonumber(L, 4); CVector pos = { x, y, z };// вектор для координат.
			ped->SetObjective(OBJECTIVE_SPRINT_TO_AREA, pos);// пед делает спринт к точке.
		}
		else { throw "bad argument in function ped_run_to_point"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int ped_walk_to_point(lua_State* L) {// пед идет пешком.
 	cpp_trace("ped_walk_to_point: ВХОД");
 	try {
 		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
 			LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {// указатель на педа.

 			const void* p = lua_topointer(L, 1);
 			CPed* ped = findpedinpool(p);// получить указатель на педа.
 			if (ped == NULL) { cpp_trace("ped_walk_to_point: пед НЕ найден в пуле, ВЫХОД"); return 0; }
 			float x = lua_tonumber(L, 2);	float y = lua_tonumber(L, 3);
 			float z = lua_tonumber(L, 4);
			cpp_tracef("ped_walk_to_point: пед=%p в пуле, m_bInVehicle=%d, m_pVehicle=%p, точка (%.2f, %.2f, %.2f)",
				ped, (int)ped->m_bInVehicle, (void*)ped->m_pVehicle, x, y, z);
 			warp_ped_out_of_car(ped);// пед ещё в авто (не успел выйти)? высадить принудительно, иначе SetObjective роняет игру.
			cpp_tracef("ped_walk_to_point: после warp_ped_out_of_car m_bInVehicle=%d setobjective", (int)ped->m_bInVehicle);
 			CVector pos = { x, y, z };
 			ped->SetObjective(OBJECTIVE_GOTO_AREA_ON_FOOT, pos);// пед идет пешком.
 			cpp_trace("ped_walk_to_point: SetObjective OK");
 		}
 		else { throw "bad argument in function ped_walk_to_point"; }
 	}
 	catch (const char* x) { writelog(x); }// записать ошибку в файл.
 };

int ped_run_to_point(lua_State* L) {// пед бежит к точке пешком.
 	cpp_trace("ped_run_to_point: ВХОД");
 	try {
 		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
 			LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {// указатель на педа.

 			const void* p = lua_topointer(L, 1);
 			CPed* ped = findpedinpool(p);// получить указатель на педа.
 			if (ped == NULL) { cpp_trace("ped_run_to_point: пед НЕ найден в пуле, ВЫХОД"); return 0; }
 			float x = lua_tonumber(L, 2);	float y = lua_tonumber(L, 3);
 			float z = lua_tonumber(L, 4);
			cpp_tracef("ped_run_to_point: пед=%p m_bInVehicle=%d m_pVehicle=%p точка (%.2f, %.2f, %.2f)",
				ped, (int)ped->m_bInVehicle, (void*)ped->m_pVehicle, x, y, z);
 			warp_ped_out_of_car(ped);// пед ещё в авто (не успел выйти)? высадить принудительно, иначе SetObjective роняет игру.
 			CVector pos = { x, y, z };
 			ped->SetObjective(OBJECTIVE_RUN_TO_AREA, pos);// пед бежит к точке пешком.
 			cpp_trace("ped_run_to_point: SetObjective OK");
 		}
 		else { throw "bad argument in function ped_run_to_point"; }
 	}
 	catch (const char* x) { writelog(x); }// записать ошибку в файл.
 };

int getobjangle(lua_State* L) {// получить угол объекта.
	try {
		float angle;
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на объект.

			const void* p = lua_topointer(L, 1);
			CObject* obj = findobjinpool(p);// получить указатель на объект.
			angle = obj->GetHeading() * 57.295779f;// получить угол объекта. новый SDK возвращает радианы, переводим в градусы для lua.
			//Command<COMMAND_GET_OBJECT_HEADING>(CPools::GetObjectRef(obj), angle);
			lua_pushnumber(L, angle);// отправить в стек.
			return 1;
		}// получить угол объекта.

		else { throw "bad argument in function getobjangle"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int getpedangle(lua_State* L) {// получить угол педа
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			float angle;// переменная хранить угол педа.
			angle = ped->GetHeading() * 57.295779f;// получить угол педа. новый SDK возвращает радианы, переводим в градусы для lua.
			lua_pushinteger(L, angle);// отправить в стек.
			return 1;
			/*CPed* player = FindPlayerPed();// найти игрока.
			if (ped == player) {
				Command<COMMAND_GET_PLAYER_HEADING>(CWorld::PlayerInFocus, &angle);//  получить угол педа.
				lua_pushinteger(L, angle);// отправить в стек.
				return 1;
			}
			else {
				Command<COMMAND_GET_CHAR_HEADING>(CPools::GetPedRef(ped), &angle);//  получить угол педа.
				lua_pushinteger(L, angle);// отправить в стек.
				return 1;
			}*/
		}
		else { throw "bad argument in function getpedangle option of the player"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int getpedcoordinates_on_x(lua_State* L) {// Получить мировую координату по x для педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			float x = lua_tonumber(L, 2);
			CVector pos = ped->pos;
			pos += ped->right * x;
			lua_pushnumber(L, pos.x); lua_pushnumber(L, pos.y); lua_pushnumber(L, pos.z);
			return 3;
		}
		else { throw "bad argument in function getpedcoordinates_on_x"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int getpedcoordinates_on_y(lua_State* L) {// // Получить мировую координату по y для педа.
 	try {
 		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на педа.

 			const void* p = lua_topointer(L, 1);
 			CPed* ped = findpedinpool(p);// получить указатель на педа.
 			if (ped == NULL) { cpp_trace("getpedcoordinates_on_y: пед НЕ найден в пуле, ВЫХОД"); return 0; }

 			float y = lua_tonumber(L, 2);
 			CVector pos = ped->pos;
 			pos += ped->up * y;   lua_pushnumber(L, pos.x);
 			lua_pushnumber(L, pos.y);   lua_pushnumber(L, pos.z);
			cpp_tracef("getpedcoordinates_on_y: пед=%p pos=(%.2f, %.2f, %.2f) сдвиг по up * %0.1f -> (%.2f, %.2f, %.2f)",
				ped, ped->pos.x, ped->pos.y, ped->pos.z, y, pos.x, pos.y, pos.z);
 			return 3;
 		}
 		else { throw "bad argument in function getpedcoordinates_on_y"; }
 	}
 	catch (const char* x) { writelog(x); }// записать ошибку в файл.
 };

int getcarcoordinates_on_x(lua_State* L) {// Получить мировую координату по x для авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на авто.

			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.
			if (car == NULL) return 0;

			float x = lua_tonumber(L, 2);
			CVector pos = car->pos;
			pos += car->right * x;
			lua_pushnumber(L, pos.x); lua_pushnumber(L, pos.y); lua_pushnumber(L, pos.z);
			return 3;
		}
		else { throw "bad argument in function getcarcoordinates_on_x"; }
	}
	catch (const char* x) { writelog(x); }
};

int getcarcoordinates_on_y(lua_State* L) {// Получить мировую координату по y для авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на авто.

			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			if (car == NULL) return 0;

			float y = lua_tonumber(L, 2);
			CVector pos = car->pos;
			pos += car->up * y; lua_pushnumber(L, pos.x);
			lua_pushnumber(L, pos.y); lua_pushnumber(L, pos.z);
			return 3;
		}
		else { throw "bad argument in function getcarcoordinates_on_y"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int worldcoord(lua_State* L) {// Перевод в мировые координаты.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)
			&& LUA_TNUMBER == lua_type(L, 3)) {// указатель на педа.

			const void* p = lua_topointer(L, 1);

			CEntity* p1 = (CEntity*)p;
			float x = lua_tonumber(L, 2); float y = lua_tonumber(L, 3);
			CVector pos = p1->pos + p1->right * x + p1->up * y;
			lua_pushnumber(L, pos.x);   lua_pushnumber(L, pos.y);
			return 2;
		}
		else { throw "bad argument in function worldcoord"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int load_requested_models(lua_State* L) {// Загрузка модели в не очереди.
	Command<COMMAND_LOAD_ALL_MODELS_NOW>(false);
	return 0;
};

int giveweaponped(lua_State* L) {// Дать оружие педу.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			if (LUA_TNUMBER == lua_type(L, 2) && (LUA_TNUMBER == lua_type(L, 3)) && (LUA_TNUMBER == lua_type(L, 4))) {

				const void* p = lua_topointer(L, 1);
				CPed* ped = findpedinpool(p);// получить указатель на педа.
				if (ped == NULL) return 0;

				unsigned int model = lua_tointeger(L, 2);// модель оружие.
				unsigned int WEAPONTYPE = lua_tointeger(L, 3);// тип оружи.
				int ammo = lua_tointeger(L, 4);// число патронов.

				CPed* player = FindPlayerPed();// найти педа
				if (ped == player) { Command<COMMAND_GIVE_WEAPON_TO_PLAYER>(CWorld::PlayerInFocus, WEAPONTYPE, ammo); }
				else {
					Command<COMMAND_GIVE_WEAPON_TO_CHAR>(CPools::GetPedRef(ped), WEAPONTYPE, ammo);// Дать оружие педу.
				}
			}
			else { throw "bad argument in function giveweaponped option weapons"; }
		}
		else { throw "bad argument in function giveweaponped option of the player"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int kill_ped_on_foot(lua_State* L) {
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {//указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);//  получить указатель на педа.
			const void* p1 = lua_topointer(L, 2);
			CPed* ped1 = findpedinpool(p1);//  получить указатель на педа.
			if (ped == NULL || ped1 == NULL) return 0;
			ped->SetObjective(OBJECTIVE_KILL_CHAR_ON_FOOT, ped1);
		}
		else { throw "bad argument in function kill_ped_on_foot option of the player"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int kill_char_any_means(lua_State* L) {
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {//указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);//  получить указатель на педа.
			const void* p1 = lua_topointer(L, 2);
			CPed* ped1 = findpedinpool(p1);//  получить указатель на педа.
			if (ped == NULL || ped1 == NULL) return 0;
			ped->SetObjective(OBJECTIVE_KILL_CHAR_ANY_MEANS, ped1);
		}
		else { throw "bad argument in function kill_char_any_means option of the ped"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_aim_at_ped(lua_State* L) {//Пед целиться в педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {// указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			const void* p1 = lua_topointer(L, 2);
			CPed* ped2 = findpedinpool(p1);// получить указатель на педа.
			ped->SetObjective(OBJECTIVE_AIM_GUN_AT, ped2);// заставить педа целиться в другого педа.
			return 0;
		}
		else { throw "bad argument in function ped_aim_at_ped option of the ped"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int is_current_weapon_ped(lua_State* L) {
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			CPed* player = FindPlayerPed();// найти педа
			int weapon_type = lua_tointeger(L, 2);// тип оружие.

			if (ped != player) {
				bool charweapontype = Command<COMMAND_IS_CURRENT_CHAR_WEAPON>(CPools::GetPedRef(ped), weapon_type);
				lua_pushboolean(L, charweapontype);
				return 1;
			}
			else {
				bool playerweapontype = Command<COMMAND_IS_CURRENT_PLAYER_WEAPON>(CWorld::PlayerInFocus, weapon_type);
				lua_pushboolean(L, playerweapontype);// отправить булевое значение сравнением с текущим оружие педа.
				return 1;
			}
		}
		else { throw "bad argument in function is_current_weapon_ped"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int create_sphere(lua_State* L) {//создать сферу.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {
			float x = lua_tonumber(L, 1);  float y = lua_tonumber(L, 2);
			float z = lua_tonumber(L, 3);  float radius = lua_tonumber(L, 4);// радиус.
			int sphere;// переменная, которая хранить id сферы.
			CVector pos = { x, y, z };
			Command<COMMAND_ADD_SPHERE>(pos.x, pos.y, pos.z, radius, &sphere); //создать, удалить, создать сферу
			Command<COMMAND_REMOVE_SPHERE>(sphere);// нужно, чтобы обойти глюк.
			Command<COMMAND_ADD_SPHERE>(pos.x, pos.y, pos.z, radius, &sphere);	//lua_settop(L, 0);// очистить стек.
			spheres.emplace(sphere, L);
			lua_pushinteger(L, sphere);// отправить id сферы в стек.
			return 1;
		}
		else { throw "bad argument in function create_sphere "; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int create_money_pickup(lua_State* L) {//создать пачку денег.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {
			int money = lua_tointeger(L, 1); // количество денег в пикапе
			float x = lua_tonumber(L, 2);
			float y = lua_tonumber(L, 3); float z = lua_tonumber(L, 4);
			int idpickup;// переменная, которая хранить id пикапа денег.
			CVector pos = { x, y, z };
			Command<COMMAND_CREATE_MONEY_PICKUP>(pos.x, pos.y, pos.z, money, &idpickup);
			pickupsids.emplace(idpickup, L);
			lua_pushinteger(L, idpickup);// отправить id пикапа в стек.
			return 1;
		}
		else { throw "bad argument in function create_money_pickup"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int create_weapon_pickup(lua_State* L) {//создать пикап оружие.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)
			&& LUA_TNUMBER == lua_type(L, 4) && LUA_TNUMBER == lua_type(L, 5) && LUA_TNUMBER == lua_type(L, 6)) {
			int model = lua_tointeger(L, 1);// модель оружие.
			int type = lua_tointeger(L, 2);// тип пикапа.
			int ammo = lua_tointeger(L, 3); // начальное кол-во патронов в оружие.
			float x = lua_tonumber(L, 4);
			float y = lua_tonumber(L, 5); float z = lua_tonumber(L, 6);
			int idpickup;// переменная, которая хранить id пикапа.
			CVector pos = { x, y, z };
			Command<COMMAND_CREATE_PICKUP_WITH_AMMO>(model, type, ammo, pos.x, pos.y, pos.z, &idpickup);
			pickupsids.emplace(idpickup, L);// добавить в map пикапов.
			lua_pushinteger(L, idpickup);// отправить id пикапа в стек.
			return 1;
		}
		else { throw "bad argument in function create_weapon_pickup"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int create_pickup(lua_State* L) {//создать пикап.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)
			&& LUA_TNUMBER == lua_type(L, 4) && LUA_TNUMBER == lua_type(L, 5)) {

			int model = lua_tointeger(L, 1); // модель пикапа.
			int type = lua_tointeger(L, 2);// тип пикапа.
			float x = lua_tonumber(L, 3);	float y = lua_tonumber(L, 4);
			float z = lua_tonumber(L, 5);
			int idpickup;// переменная, которая хранить id пикапа.
			CVector pos = { x, y, z };
			Command<COMMAND_CREATE_PICKUP>(model, type, pos.x, pos.y, pos.z, &idpickup);
			pickupsids.emplace(idpickup, L);// добавить в map пикапов.
			lua_pushinteger(L, idpickup);// отправить id пикапа в стек.
			return 1;
		}
		else { throw "bad argument in function create_weapon_pickup"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int remove_sphere(lua_State* L) {// удалить сферу.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// значение число.
			int sphere = lua_tointeger(L, 1);
			Command<COMMAND_REMOVE_SPHERE>(sphere);// удалить сферу.
			auto it = spheres.find(sphere);
			if (it != spheres.end() && L == it->second) {
				spheres.erase(it);
			}
			return 0;
		}
		else { throw "bad argument in function remove_sphere"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int picked_up(lua_State* L) {// проверка пикап подобран.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// значение id пикапа.
			int pickup = lua_tointeger(L, 1);
			bool checkpickup = Command<COMMAND_HAS_PICKUP_BEEN_COLLECTED>(pickup);
			lua_pushboolean(L, checkpickup);
			return 1;
		}
		else { throw "bad argument in function picked_up"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int remove_pickup(lua_State* L) {// удалить пикап.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// значение id пикапа.
			int pickup = lua_tointeger(L, 1);
			Command<COMMAND_REMOVE_PICKUP>(pickup);// удалить пикап.
			auto it = pickupsids.find(pickup);
			if (it != pickupsids.end() && L == it->second) {
				pickupsids.erase(it);
			}
			return 0;
		}
		else { throw "bad argument in function remove_pickup"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int remove_car(lua_State* L) {// удалить авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.

			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.
			if (car == NULL) { writelog("remove_car: car is NULL"); return 0; }

			warp_out_car_occupants(car);// выкинуть из авто всех сидящих (включая игрока).
			Command<COMMAND_MARK_CAR_AS_NO_LONGER_NEEDED>(CPools::GetVehicleRef(car));// удалить авто.
			auto it = mapcars.find(car);
			if (it != mapcars.end() && L == it->second) {
				mapcars.erase(it);
			}
			return 0;
		}
		else { throw "bad argument in function remove_car"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int remove_obj(lua_State* L) {// удалить объект.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// значение объект.
			const void* p = lua_topointer(L, 1);
			CObject* obj = findobjinpool(p);// получить указатель на объект.
			if (obj != NULL) {//obj->Remove();
				Command<COMMAND_DELETE_OBJECT>(CPools::GetObjectRef(obj));// удалить объект.
				auto it = mapobjs.find(obj);
				if (it != mapobjs.end() && L == it->second) {
					mapobjs.erase(it);
				}
			}
			return 0;
		}
		else { throw "bad argument in function remove_obj"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int car_in_water(lua_State* L) {// проверка авто в воде.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.

			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p); // получить указатель на авто.

			bool check_in_wanter = Command<COMMAND_IS_CAR_IN_WATER>(CPools::GetVehicleRef(car));
			lua_pushboolean(L, check_in_wanter);
			return 1;
		}
		else { throw "bad argument in function car_in_water"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_wanted(lua_State* L) {// уcтановить уровень розыска.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// значение число.
			int wanted = lua_tointeger(L, 1);// кол-во звезд розыска.
			CWanted::SetMaximumWantedLevel(wanted);// из SDK (раньше сырой вызов 0x4D1E90).	Command<COMMAND_SET_MAX_WANTED_LEVEL>(wanted);
			/*
			(void(__cdecl *)(const wchar_t *text, unsigned time, bool flag1))0x584300;
			(*(void (__thiscall*)(CObject*))0x4E3690)(_this);
			int __cdecl CWanted::SetMaximumWantedLevel(int a1)
			*/
			CPlayerPed& player1 = *FindPlayerPed();
			player1.SetWantedLevel(wanted);
			player1.SetWantedLevelNoDrop(wanted);
			return 0;
		}
		else { throw "bad argument in function set_wanted"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_widescreen(lua_State* L) {// вкл/выкл широкий экран.
	try {
		if (LUA_TBOOLEAN == lua_type(L, 1)) {// значение булевое.
			bool switc = lua_toboolean(L, 1);//
			Command<COMMAND_SWITCH_WIDESCREEN>(switc);
			return 0;
		}
		else { throw "bad argument in function set_widescreen"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int is_wanted_level(lua_State* L) {// проверить уровень розыска.
	static int delay = 0;
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// значение число.
			int wanted = lua_tointeger(L, 1); wanted--;
			if (delay == 0) {
				delay = iters;
				lua_pushboolean(L, false);
				return 1;
			}
			else {
				if (iters - delay > 5) {
					delay = 0;
					lua_pushboolean(L, Command<COMMAND_IS_WANTED_LEVEL_GREATER>(CWorld::PlayerInFocus, wanted));
					return 1;
				}
			}
		}
		else { throw "bad argument in function is_wanted_level"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.

	return 0;
};

int wanted_level(lua_State* L) {// получить уровень розыска.

	CPlayerPed& player1 = *FindPlayerPed();
	int wanted = player1.GetWantedLevel();
	lua_pushinteger(L, wanted);
	return 1;
};

int clear_wanted(lua_State* L) {// убрать уровень розыска.
	CPlayerPed& player1 = *FindPlayerPed();
	player1.SetWantedLevel(0);
	//Command<COMMAND_CLEAR_WANTED_LEVEL>(CWorld::PlayerInFocus);
	return 0;
};

int remove_ped(lua_State* L) {// удалить педа.
 	cpp_trace("remove_ped: ВХОД");
 	try {
 		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.

 			const void* p = lua_topointer(L, 1);
 			CPed* ped = findpedinpool(p);// получить указатель на педа.
 			if (ped == NULL) { cpp_trace("remove_ped: пед НЕ найден в пуле, ВЫХОД"); return 0; }
 			cpp_tracef("remove_ped: пед=%p m_bInVehicle=%d m_pVehicle=%p health=%.1f", ped, (int)ped->m_bInVehicle, (void*)ped->m_pVehicle, ped->m_fHealth);
 			warp_ped_out_of_car(ped);// пед сидит в авто? высадить перед удалением.
 			ped->ClearInvestigateEvent();// пед уходит, опустить педа.
 			Command<COMMAND_MARK_CHAR_AS_NO_LONGER_NEEDED>(CPools::GetPedRef(ped));// удалить педа.
 			cpp_trace("remove_ped: MARK_CHAR_AS_NO_LONGER_NEEDED OK");
 			auto it = mappeds.find(ped);
 			if (it != mappeds.end() && L == it->second) {
 				mappeds.erase(it);
 			}
 			return 0;
 		}
		else { throw "bad argument in function remove_ped"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int remove_spec_ped(lua_State* L) {// удалить спец педа.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// значение пед.

			int idped = lua_tointeger(L, 1);
			CStreaming::SetMissionDoesntRequireSpecialChar(idped); // 0296: unload_special_actor 21
			return 0;
		}
		else { throw "bad argument in function remove_spec_ped"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int kill_ped(lua_State* L) {// убить педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// значение число.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			Command<COMMAND_EXPLODE_CHAR_HEAD>(CPools::GetPedRef(ped));// убить педа.
			return 0;
		}
		else { throw "bad argument in function kill_ped"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int setpedcoordes(lua_State* L) {// установить координаты для педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)
			&& LUA_TNUMBER == lua_type(L, 4)) {//число.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.

			float x = lua_tonumber(L, 2);
			float y = lua_tonumber(L, 3);
			float z = lua_tonumber(L, 4);
			CPed* player = FindPlayerPed();// найти педа
			if (ped != player) {
				Command<COMMAND_SET_CHAR_COORDINATES>(CPools::GetPedRef(ped), x, y, z);
				return 0;
			}
			else {
				Command<COMMAND_SET_PLAYER_COORDINATES>(CWorld::PlayerInFocus, x, y, z);
				return 0;
			}
		}
		else { throw "bad argument in function setpedcoordes"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int create_marker_obj(lua_State* L) {//создать маркер над объектом.
	int marker;// id маркера.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на объект.
			const void* p = lua_topointer(L, 1);
			CObject* obj = findobjinpool(p);// получить указатель на педа.

			Command<COMMAND_ADD_BLIP_FOR_OBJECT>(CPools::GetObjectRef(obj), &marker);//создать маркер над объектом.
			markeron.emplace(marker, L);// добавить в map для маркеров.
			lua_pushinteger(L, marker);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function create_marker_obj"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int move_obj(lua_State* L) {//двигать объект.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)
			&& LUA_TNUMBER == lua_type(L, 4) && LUA_TNUMBER == lua_type(L, 5) && LUA_TNUMBER == lua_type(L, 6)
			&& LUA_TNUMBER == lua_type(L, 7) && LUA_TNUMBER == lua_type(L, 8)) {// указатель на объект.
			const void* p = lua_topointer(L, 1);
			CObject* obj = findobjinpool(p);// получить указатель на объект.
			float x = lua_tonumber(L, 2); float y = lua_tonumber(L, 3);
			float z = lua_tonumber(L, 4);
			float speedx = lua_tonumber(L, 5);//
			float speedy = lua_tonumber(L, 6);// скорости по осам.
			float speedz = lua_tonumber(L, 7);//
			int collision_check = lua_tointeger(L, 8);// вкл/выкл коллизию объекта.
			Command<COMMAND_SLIDE_OBJECT>(CPools::GetObjectRef(obj), x, y, z, speedx, speedy, speedz, collision_check);

			return 0;
		}
		else { throw "bad argument in function move_obj"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int move_rotate(lua_State* L) {//вращать объект.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на объект.
			lua_settop(L, 4);

			const void* p = lua_topointer(L, 1);
			CObject* obj = findobjinpool(p);// получить указатель на педа.
			float Angle1 = lua_tonumber(L, 2); float Angle2 = lua_tonumber(L, 3);// углы вращения объекта.
			int flag = lua_tointeger(L, 4);
			Command<COMMAND_ROTATE_OBJECT>(CPools::GetObjectRef(obj), Angle1, Angle1, flag);

			lua_settop(L, 0);
			return 0;
		}
		else { throw "bad argument in function move_obj"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int getobjcoordes(lua_State* L) {// получить координаты объекта.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.

			const void* p = lua_topointer(L, 1);
			CObject* obj = findobjinpool(p);// получить указатель на объект.

			lua_pushnumber(L, obj->GetPosition().x);// отправить в стек.
			lua_pushnumber(L, obj->GetPosition().y);// отправить в стек.
			lua_pushnumber(L, obj->GetPosition().z);// отправить в стек.
			return 3;
		}// получить координаты объект.

		else { throw "bad argument in function getobjcoordes"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int getobjcoordinates_on_x(lua_State* L) {// Получить мировую координату по x для объекта.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на объекта.

			const void* p = lua_topointer(L, 1);
			CObject* obj = findobjinpool(p);// получить указатель на объект.

			float x = lua_tonumber(L, 2);
			CVector pos = obj->pos;
			pos += obj->right * x;
			lua_pushnumber(L, pos.x); lua_pushnumber(L, pos.y); lua_pushnumber(L, pos.z);
			return 3;
		}
		else { throw "bad argument in function getobjcoordinates_on_x"; }
	}
	catch (const char* x) {
		writelog(x);
	}
};

int getobjcoordinates_on_y(lua_State* L) {// Получить мировую координату по y для объекта.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на объект.

			const void* p = lua_topointer(L, 1);
			CObject* obj = findobjinpool(p);// получить указатель на объект.

			float y = lua_tonumber(L, 2);
			CVector pos = obj->pos;
			pos += obj->up * y; lua_pushnumber(L, pos.x);
			lua_pushnumber(L, pos.y);   lua_pushnumber(L, pos.z);
			return 3;
		}
		else { throw "bad argument in function getobjcoordinates_on_y"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int ped_in_point_in_radius(lua_State* L) {// проверить находится пед в координатах с радиусом.
	static int delay = 0;
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)
			&& LUA_TNUMBER == lua_type(L, 4) && LUA_TNUMBER == lua_type(L, 5) && LUA_TNUMBER == lua_type(L, 6)
			&& LUA_TNUMBER == lua_type(L, 6)) {//строка.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			if (ped == NULL) { lua_pushboolean(L, false); return 1; }

			float x1 = lua_tonumber(L, 2);	float y1 = lua_tonumber(L, 3);	float z1 = lua_tonumber(L, 4);
			float rx = lua_tonumber(L, 5);	float ry = lua_tonumber(L, 6);	float rz = lua_tonumber(L, 7);

			this_thread::sleep_for(chrono::milliseconds(10));
			float x = ped->GetPosition().x;
			float y = ped->GetPosition().y;
			float z = ped->GetPosition().z;
			float r2 = rx * rx + ry * ry + rz * rz;
			x = x - x1; y = y - y1; z = z - z1;
			float res = x * x + y * y + z * z;
			if (res < r2) {

				lua_pushboolean(L, true);
				return 1;
			}
			else {
				lua_pushboolean(L, false);
				return 1;
			}
		}
		else { throw "bad argument in function ped_in_point_in_radius"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int car_in_point_in_radius(lua_State* L) {// проверить находится авто в координатах с радиусом.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)
			&& LUA_TNUMBER == lua_type(L, 4) && LUA_TNUMBER == lua_type(L, 5)) {//строка.

			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.
			if (car == NULL) { lua_pushboolean(L, false); return 1; }

			float x = lua_tonumber(L, 2);
			float y = lua_tonumber(L, 3);
			float z = lua_tonumber(L, 4);
			float radius = lua_tonumber(L, 5);// радиус.
			lua_pop(L, lua_gettop(L));
			bool check = car->IsSphereTouchingVehicle(x, y, z, radius);
			lua_pushboolean(L, check);		//lua_pushboolean(L, Command<COMMAND_LOCATE_CAR_3D>(CPools::GetVehicleRef(car), x, y, z, rx, ry, rz));
			return 1;
		}
		else { throw "bad argument in function car_in_point_in_radius"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

void dellod() {// удалить лог ошибок.
	string path = "lualoader\\log.txt";
	fstream f1; {f1.open(path, fstream::in | fstream::out | fstream::app);
	if (f1.is_open()) {// если файл есть удалить.
		f1.close();	remove("lualoader\\log.txt");
	}
	}
};

int cleanstl() {//удаления объектов из всех stl.
	if (!markeron.empty()) {// если не пусть.
		markeron.clear();// маркеры
	}
	if (!spheres.empty()) {// если не пусть.
		spheres.clear();// сферы.
	}
	if (!pickupsids.empty()) {// если не пусть.
		pickupsids.clear();//пикапы.
	}
	if (!firesids.empty()) {// если не пусть.
		firesids.clear();//огонь.
	}
	if (!mapcars.empty()) {// если не пусть.
		mapcars.clear();//авто.
	}
	if (!mapobjs.empty()) {// если не пусть.
		mapobjs.clear();//объект.
	}

	return 0;
};

int getflagmission(lua_State* L) {// проверка флага миссии.
 	unsigned int& OnAMissionFlag = *(unsigned int*)0x978748;
 	bool getflagmission = (CTheScripts::ScriptSpace[OnAMissionFlag]);
 	lua_pushboolean(L, getflagmission);// получить флаг миссии.
 	cpp_tracef("getflagmission = %s", getflagmission ? "true" : "false");
 	return 1;
 };

int setflagmission(lua_State* L) {// уcтановить флага миссии.
 	unsigned int& OnAMissionFlag = *(unsigned int*)0x978748;
 	try {
 		if (LUA_TBOOLEAN == lua_type(L, 1)) {// значение число.
 			bool flag = lua_toboolean(L, 1);
 			cpp_tracef("setflagmission = %s", flag ? "true" : "false");
			CTheScripts::ScriptSpace[OnAMissionFlag] = flag;
			return 0;
		}
		else { throw "bad argument in function setflagmission"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int showtext(lua_State* L) {// Вывод особого текста на экран.
	try {
		if (LUA_TSTRING == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {//строка.
			const char* c = lua_tostring(L, 1);
			int time = lua_tointeger(L, 2);// время вывода текста.
			int style = lua_tointeger(L, 3);// стиль текста.
wchar_t* str = getwchat(c);
 			CMessages::AddBigMessage(str, time, style);/*0 большими, розовые как миссии пройдена,
 			1 надпись как названия миссии, 2 зеленым большими*/
 			cpp_tracef("showtext: %s (time=%d style=%d)", c, time, style);
 			return 0;
 		}
		else { throw "bad argument in function showtext"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int remove_blip(lua_State* L) {// удалить метку с карты.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// значение число.
			int blip = lua_tointeger(L, 1);// получить id метки.
			Command<COMMAND_REMOVE_BLIP>(blip);// удалить метку на карте.

			auto it = markeron.find(blip);
			if (it != markeron.end() && L == it->second) {
				markeron.erase(it);
			}

			return 0;
		}
		else { throw "bad argument in function remove_blip"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int createblip(lua_State* L) {// создать метку карте.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {// значение число.
			int point;// id метке.
			int t = lua_tointeger(L, 1);// id метки на карте.
			float x = lua_tonumber(L, 2); float y = lua_tonumber(L, 3);
			float z = lua_tonumber(L, 4); CVector p = { x, y, z };
			Command<COMMAND_ADD_SHORT_RANGE_SPRITE_BLIP_FOR_CONTACT_POINT>(p.x, p.y, p.z, t, &point);
			lua_pushinteger(L, point);// отправить в стек и получить из стека можно
			markeron.emplace(point, L);// добавить в map для маркеров.
			return 1;
		}// int

		else { throw "bad argument in function createblip"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int createmarker(lua_State* L) {// создать маркер на карте.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)
			&& LUA_TNUMBER == lua_type(L, 4) && LUA_TNUMBER == lua_type(L, 5)) {// значение число.
			int point; //id маркера.
			int t = lua_tointeger(L, 1);// тип маркер.
			int size = lua_tointeger(L, 2);// размер маркера на карте.
			float x = lua_tonumber(L, 3); float y = lua_tonumber(L, 4);
			float z = lua_tonumber(L, 5); CVector p = { x, y, z };
			Command<COMMAND_ADD_BLIP_FOR_COORD_OLD>(p.x, p.y, p.z, t, size, &point);
			markeron.emplace(point, L);// добавить в map для маркеров.
			lua_pushinteger(L, point);// отправить в стек и получить из стека можно
			return 1;
		}// int

		else { throw "bad argument in function createmarker"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int play_sound(lua_State* L) {// проиграть мелодию.
 	try {
 		if (LUA_TNUMBER == lua_type(L, 1)) {// значение число.
 			int music = lua_tointeger(L, 1);// получить id мелодии.
 			cpp_tracef("play_sound %d", music);
 			Command<COMMAND_PLAY_MISSION_PASSED_TUNE>(music);
 			return 0;
 		}
 		else { throw "bad argument in function play_sound"; }
 	}
 	catch (const char* x) { writelog(x); }// записать ошибку в файл.
 	return 0;
 };

int create_marker_pickup(lua_State* L) {// создать маркер над пикапом.
	try {
		int marker;//id маркера.
		if (LUA_TNUMBER == lua_type(L, 1)) {// значение число.
			int pickup = lua_tointeger(L, 1);// получить id пикапа.
			Command<COMMAND_ADD_BLIP_FOR_PICKUP>(pickup, &marker);
			markeron.emplace(marker, L);// добавить в map для маркеров.
			lua_pushinteger(L, marker);// отправить в стек и получить из стека можно
			return 1;
		}
		else { throw "bad argument in function play_sound"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int play_voice(lua_State* L) {// проиграть реплику педа.
	try {
		if (LUA_TSTRING == lua_type(L, 1)) {// значение число.
			const char* voice = lua_tostring(L, 1);
			Command<COMMAND_LOAD_MISSION_AUDIO>(1, voice);// загрузить реплику.
			while (true) {
				this_thread::sleep_for(chrono::milliseconds(1));
				if (Command<COMMAND_HAS_MISSION_AUDIO_LOADED>(1)) {
					Command<COMMAND_PLAY_MISSION_AUDIO>(1);
					if (Command<COMMAND_HAS_MISSION_AUDIO_FINISHED>(1)) {
						Command<COMMAND_CLEAR_MISSION_AUDIO>(1);
						break;
					}
				}
			}
			return 0;
		}
		else { throw "bad argument in function play_voice"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int createped(lua_State* L) {// создать педа.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)
			&& LUA_TNUMBER == lua_type(L, 4) && LUA_TNUMBER == lua_type(L, 5)) {// значение число.
			int model = lua_tointeger(L, 1);// модель педа
			int type = lua_tointeger(L, 2);// тип педа.
			float x = lua_tonumber(L, 3); float y = lua_tonumber(L, 4); float z = lua_tonumber(L, 5);
			CVector pos = { x, y, z };
			CPed* ped = nullptr;
			Command<COMMAND_CREATE_CHAR>(type, model, pos.x, pos.y, pos.z, &ped);
			mappeds.emplace(ped, L);// добавить map для педов.
			lua_pushlightuserdata(L, ped);// отправить в стек и получить из стека можно.
			return 1;
		}// int

		else { throw "bad argument in function createped"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

void load_model_before_avalible(int model) {
	cpp_tracef("load_model_before_avalible: ЗАПРОС модели id=%d", model);
	Command<COMMAND_LOAD_ALL_MODELS_NOW>(false);
	Command<COMMAND_REQUEST_MODEL>(model);
	Command<COMMAND_LOAD_ALL_MODELS_NOW>(false);
	int guard = 0;
	while (!Command<COMMAND_HAS_MODEL_LOADED>(model)) {
		this_thread::sleep_for(chrono::milliseconds(1));// задержка
		Command<COMMAND_REQUEST_MODEL>(model);
		if (++guard % 3000 == 0) cpp_tracef("load_model_before_avalible: ЖДЁМ модель id=%d (~%d мс)", model, guard);
	}
	cpp_tracef("load_model_before_avalible: модель id=%d загружена", model);
};

int create_spec_ped(lua_State* L) {// создать спец педа.
	try {
		if (LUA_TSTRING == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {// значение число.
			//int idmodel = lua_tointeger(L, -7);// спец id пед.
			//int specmodel = lua_tointeger(L, -6);// модель педа.
			//int type = lua_tointeger(L, -5);// тип педа.
			//int slot  = lua_tointeger(L, -4);// слот педа.
			CPed* ped = NULL;
			char const* model = lua_tostring(L, 1);// модель, например, "sam".
			float x = lua_tonumber(L, 2); float y = lua_tonumber(L, 3);
			float z = lua_tonumber(L, 4);
			CVector pos = { x, y, z };
			int idped = 0;
			load_model_before_avalible(idped);
			Command<COMMAND_CREATE_CHAR>(4, idped, pos.x, pos.y, pos.z, &ped);
			//Command<COMMAND_CREATE_PLAYER>(0, pos.x, pos.y, pos.z, &ped);
			if (ped == NULL) { writelog("create_spec_ped: CREATE_CHAR вернул NULL"); lua_pushnil(L); return 1; }

			CPed* p = findpedinpool(ped);// получить указатель на педа.

			Command<COMMAND_MARK_MODEL_AS_NO_LONGER_NEEDED>(idped);
			mappeds.emplace(ped, L);// добавить map для педов.


			Command<COMMAND_LOAD_ALL_MODELS_NOW>(false);
			Command<COMMAND_UNDRESS_CHAR>(CPools::GetPedRef(p), model);
			Command<COMMAND_LOAD_ALL_MODELS_NOW>(false);
			Command<COMMAND_DRESS_CHAR>(CPools::GetPedRef(p));
			lua_pushlightuserdata(L, ped);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function create_spec_ped"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int isped(lua_State* L) {// проверка это пед?.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// значение число.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.

			if (ped != NULL) {
				lua_pushboolean(L, true);
				return 1;
			}

			else {
				lua_pushboolean(L, false);
				return 1;
			}
		}
		else { throw "bad argument in function isped"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int isvehicle(lua_State* L) {// проверка это транспорт?.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// значение число.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на педа.

			if (car != NULL) {
				lua_pushboolean(L, true);
				return 1;
			}

			else {
				lua_pushboolean(L, false);
				return 1;
			}

		}
		else { throw "bad argument in function isvehicle"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int isobject(lua_State* L) {// проверка это объект?.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// значение число.
			const void* p = lua_topointer(L, 1);
			CObject* obj = findobjinpool(p);// получить указатель на объект.

			if (obj != NULL) {
				lua_pushboolean(L, true);
				return 1;
			}

			else {
				lua_pushboolean(L, false);
				return 1;
			}
		}
		else { throw "bad argument in function isobject"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int fade(lua_State* L) {//затенение, просветления.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && (LUA_TNUMBER == lua_type(L, 2))) {
			int lihgt = lua_tointeger(L, 1);// Время.
			int time = lua_tointeger(L, 2);// свет = 1 тьма = 0.
			Command<COMMAND_DO_FADE>(time, lihgt);// затенение.

			//CCamera* cam;
			//cam = &TheCamera;// камера из SDK (раньше хардкод 0x7E4688).
			//cam->Fade(time, lihgt);// затенение.
			return 0;
		}
		else { throw "bad argument in function fade"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int setsizemarker(lua_State* L) {// установить размер маркера.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && (LUA_TNUMBER == lua_type(L, 2))) {
			int marker = lua_tointeger(L, 1);// Время.
			int size = lua_tointeger(L, 2);// размер маркера.
			Command<COMMAND_CHANGE_BLIP_SCALE>(marker, size);
			return 0;
		}
		else { throw "bad argument in function setsizemarker"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int sound_coordinate(lua_State* L) {// Проиграть звук в координатах
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {// значение число.

			int sound = lua_tointeger(L, 1);// id звука.
			float x = lua_tonumber(L, 2); float y = lua_tonumber(L, 3);
			float z = lua_tonumber(L, 4);
			Command<COMMAND_ADD_ONE_OFF_SOUND>(x, y, z, sound);// Проиграть звук в координатах.  lua_settop(L, 0);
			return 0;
		}// int

		else { throw "bad argument in function sound_coordinate"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int load_scene(lua_State* L) {// загрузить модели на координатах заранее.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)
			&& LUA_TNUMBER == lua_type(L, 3)) {// значение число.

			float x = lua_tonumber(L, 1); float y = lua_tonumber(L, 2);
			float z = lua_tonumber(L, 3);
			Command<COMMAND_LOAD_SCENE>(x, y, z);// загрузить область в координатах
			lua_settop(L, 0);
			return 0;
		}// int

		else { throw "bad argument in function load_scene"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int show_text_styled(lua_State* L) {// вывести игровой текст.
	try {
		if (LUA_TSTRING == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)
			&& LUA_TNUMBER == lua_type(L, 3)) {// значение число.
			const char* text = lua_tostring(L, 1);// текст.
			int time = lua_tointeger(L, 2);	int type = lua_tointeger(L, 3);
			Command<COMMAND_PRINT_BIG>(text, time, type);// числа для старта гонки.
			lua_settop(L, 0);// очистить стек.
			return 0;
		}

		else { throw "bad argument in function show_text_styled"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int show_text_gtx(lua_State* L) {// вывести игровой текст.
	try {
		if (LUA_TSTRING == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)
			&& LUA_TNUMBER == lua_type(L, 3)) {// значение число.
			const char* text = lua_tostring(L, 1);// текст.
			int time = lua_tointeger(L, 2);	int type = lua_tointeger(L, 3);
			Command<COMMAND_LOAD_MISSION_TEXT>("GENERA1");
			Command<COMMAND_PRINT_NOW>(text, time, type);// числа для старта гонки.
			lua_settop(L, 0);// очистить стек.
			return 0;
		}

		else { throw "bad argument in function show_text_gtx"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int setcardrive(lua_State* L) {// установить водителя для авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {//строка.

			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.

			int model = lua_tointeger(L, 2);// модель педа.
			int type = lua_tointeger(L, 3);// тип педа.
			CPed* ped = nullptr;
			Command<COMMAND_CREATE_CHAR_INSIDE_CAR>(CPools::GetVehicleRef(car), type, model, &ped);
			if (ped != NULL) { mappeds.emplace(ped, L);// добавить в map педов (для destroy).
			}
			car->m_autoPilot.m_nCarMission = MISSION_NONE;
			lua_pushlightuserdata(L, ped);// отправить в стек и получить из стека можно.
			return 1;
		}
		else { throw "bad argument in function setcardrive"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int setcarpassenger(lua_State* L) {// установить пассажира для авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)
			&& LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 3)) {//число.

			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.

			int model = lua_tointeger(L, 2);
			int type = lua_tointeger(L, 3);
			int place = lua_tointeger(L, 4);//место пассажира.
			CPed* ped = nullptr;
			Command<COMMAND_CREATE_CHAR_AS_PASSENGER>(CPools::GetVehicleRef(car), type, model, place, &ped);
			if (ped != NULL) { mappeds.emplace(ped, L);// добавить в map педов (для destroy).
			}
			lua_pushlightuserdata(L, ped);
			return 1;
		}
		else { throw "bad argument in function setcarpassenger"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int setcarfirstcolor(lua_State* L) {// установить первый цвет авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {

			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.

			int firstcolor = lua_tointeger(L, 2);
			car->m_nPrimaryColor = firstcolor;// установить первый цвет авто.
			return 0;
		}
		else { throw "bad argument in function setcarfirstcolor"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int setcarseconscolor(lua_State* L) {// установить второй цвет авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);//  получить указатель на авто.
			int secondcolor = lua_tointeger(L, 2);
			car->m_nSecondaryColor = secondcolor;
			return 0;
		}// установить второй цвет авто.

		else { throw "bad argument in function setcarseconscolor"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_traffic(lua_State* L) {// установить плотноcть трафика транспорта.
 	try {//01EB
 		if (LUA_TNUMBER == lua_type(L, 1)) {// значение число.
 			float trafic = lua_tonumber(L, 1);
 			cpp_tracef("set_traffic %0.2f", trafic);
 			Command<COMMAND_SET_CAR_DENSITY_MULTIPLIER>(trafic);
 			return 0;
 		}
 		else { throw "bad argument in function set_traffic"; }
 	}
 	catch (const char* x) { writelog(x); }// записать ошибку в файл.
 	return 0;
 };

int car_explode(lua_State* L) {// взрывать авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.

			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.

			Command<COMMAND_EXPLODE_CAR>(CPools::GetVehicleRef(car));
			return 0;
		}
		else { throw "bad argument in function car_explode"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int is_car_stopped(lua_State* L) {// авто остановилось?
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.

			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.
			if (car == NULL) { lua_pushboolean(L, true); return 1; }

			lua_pushboolean(L, Command<COMMAND_IS_CAR_STOPPED>(CPools::GetVehicleRef(car)));
			return 1;
		}
		else { throw "bad argument in function is_car_stopped"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int setclock(lua_State* L) {// задать время.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {//числа.

			int hours = lua_tointeger(L, 1);// часы.
			int minutes = lua_tointeger(L, 2);// минуты.

			CClock::SetGameClock(hours, minutes);// задать время.
			return 0;
		}
		else { throw "bad argument in function setclock"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
}

int create_explosion(lua_State* L) {// Создать взрыв на координатах.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)
			&& LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 1)) {// тип взрыва и координаты.

			int tipe = lua_tointeger(L, 1);// тип взрыва.
			float x = lua_tonumber(L, 2);
			float y = lua_tonumber(L, 3);
			float z = lua_tonumber(L, 4);
			/*
			0 Стандартный средний взрыв, используется чаще всего Highslide JS
			1 Взрыв как от коктейля Молотова. По-этому после него появлятся много огня и звук тихий. Следует также учитывать то, что именно этот взрыв визуально появляется вне зависимости высоты у самой земли. Highslide JS
			2 Похож на тип 0 Highslide JS
			3 Похож на тип 0 Highslide JS
			4 Один-три взрывных линий. Следует иметь в виду, что в типах 4 и 5 не звучит грохот взрыва. Так что комбинируйте этот тип с другими типами или создавайте звук отдельно опкодом 0565 (описан выше) Highslide JS
			5 Похоже на тип 4 Highslide JS
			6 Большой взрыв, один из самых больших Highslide JS
			7 Чуть покороче чем тип 6 Highslide JS
			8 Звука нет, эффекта нет, трясение камеры есть, здоровье отнимает. Highslide JS
			9 То же самое, что и тип 8 Highslide JS
			10 Наряду с типом 6 самый большой взрыв Highslide JS
			11 Как тип 0, но чуть пожиже Highslide JS
			12 Самый маленький взрыв, как от взрыва RC-машинки. Highslide JS
			13+ Тринадцатый и последующие типы, по-видимому, не поддерживаются игрой: взрывы без звука, без эффекта.
			*/
			Command<COMMAND_ADD_EXPLOSION>(x, y, z, tipe);
			return 0;
		}
		else { throw "bad argument in function create_explosion"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_status_engine(lua_State* L) {// установить состояние двигателя авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {

			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.

			int switcher = lua_tointeger(L, 2);
			Command<COMMAND_BOAT_STOP>(CPools::GetVehicleRef(car), switcher);//
			return 0;
		}//

		else { throw "bad argument in function set_status_engine"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int player_defined(lua_State* L) {// пед существует.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);//  получить указатель на педа.

			float health = ped->m_fHealth;
			if (health > 1.0f) {
				lua_pushboolean(L, true);
				return 1;
			}
			else {
				lua_pushboolean(L, false);
				return 1;
			}
		}
		else { throw "bad argument in function player_defined"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int follow_the_leader(lua_State* L) {// //01DE / 01DF следовать за лидером
 	cpp_trace("follow_the_leader: ВХОД");
 	try {
 		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {// указатель на педа.

 			const void* p = lua_topointer(L, 1);
 			CPed* ped = findpedinpool(p);//  получить указатель на авто.
 			p = lua_topointer(L, 2);
 			CPed* ped2 = findpedinpool(p);//  получить указатель на авто.
 			if (ped == NULL || ped2 == NULL) { cpp_trace("follow_the_leader: пед/лидер НЕ в пуле"); return 0; }
 			cpp_tracef("follow_the_leader: пед=%p лидер=%p", ped, ped2);
 			ped->SetObjective(OBJECTIVE_SET_LEADER, ped2);
 			cpp_trace("follow_the_leader: SetObjective SET_LEADER OK");
 			return 0;
 		}
 		else { throw "bad argument in function follow_the_leader"; }
 	}
 	catch (const char* x) { writelog(x); }// записать ошибку в файл.
 	return 0;
 };

int arrested(lua_State* L) { // пед арестован?
 	bool ar = (CWorld::Players[CWorld::PlayerInFocus].m_nPlayerState == PLAYERSTATE_HASBEENARRESTED);
 	if (ar) {
 		cpp_trace("arrested = true");
 		lua_pushboolean(L, true);
 		return 1;
 	}
 	else {
 		lua_pushboolean(L, false);
 		return 1;
 	}
 };

int setcarcoordes(lua_State* L) {// установить координаты авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)
			&& LUA_TNUMBER == lua_type(L, 4)) {//число.
			const void* p = lua_topointer(L, 1);
			float x = lua_tonumber(L, 2);
			float y = lua_tonumber(L, 3);
			float z = lua_tonumber(L, 4);

			CVector pos = { x, y, z };
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.
			if (car == NULL) { writelog("setcarcoordes: car is NULL"); return 0; }

			Command<COMMAND_SET_CAR_COORDINATES>(CPools::GetVehicleRef(car), pos.x, pos.y, pos.z);// установить координаты авто.
		}
		else { throw "bad argument in function setcarcoordes"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int is_car_stuck(lua_State* L) {//03CE: car 12@ stuck если машина застряла.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.

			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.

			bool check_car_stuck = Command<COMMAND_IS_CAR_STUCK>(CPools::GetVehicleRef(car));
			lua_pushboolean(L, check_car_stuck);
			return 1;
		}
		else { throw "bad argument in function is_car_stuck"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int is_car_upsidedown(lua_State* L) {//01F4: car 12@ flipped если машина перевернута.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// значение число.

			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.

			bool check_car_upsidedown = Command<COMMAND_IS_CAR_UPSIDEDOWN>(CPools::GetVehicleRef(car));
			lua_pushboolean(L, check_car_upsidedown);
			return 1;
		}
		else { throw "bad argument in function is_car_upsidedown"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int is_car_upright(lua_State* L) {// 020D: car 12@ flipped если указанный автомобиль перевернут.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// значение число.

			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.

			bool check_car_upright = Command<COMMAND_IS_CAR_UPRIGHT>(CPools::GetVehicleRef(car));
			lua_pushboolean(L, check_car_upright);
			return 1;
		}
		else { throw "bad argument in function is_car_upright"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int find_road_for_car(lua_State* L) {// найти дорогу.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)
			&& LUA_TNUMBER == lua_type(L, 1)) {//число.

			float x1 = lua_tonumber(L, 1);
			float y1 = lua_tonumber(L, 2);
			float z1 = lua_tonumber(L, 3);
			float x, y, z;
			Command<COMMAND_GET_CLOSEST_CAR_NODE>(x1, y1, z1, &x, &y, &z);// найти дорогу.
			lua_pushnumber(L, x);	lua_pushnumber(L, y);	lua_pushnumber(L, z);
			return 3;
		}
		else { throw "bad argument in function find_road_for_car"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int add_stuck_car_check(lua_State* L) {// условия для того, чтобы авто считалась застрявшей.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)
			&& LUA_TNUMBER == lua_type(L, 3)) {//число.

			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.
			float distance = lua_tonumber(L, 2);// дистанция.
			int time = lua_tointeger(L, 3);// время.
			Command<COMMAND_ADD_STUCK_CAR_CHECK>(CPools::GetVehicleRef(car), distance, time);// условия для того, чтобы авто считалась застрявшей.
			return 0;
		}
		else { throw "bad argument in function add_stuck_car_check"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int setcarstrong(lua_State* L) {// сделать авто устойчивым.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {//указатель на авто.

			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.

			int value = lua_tointeger(L, 2);
			if (value == 1) {
				Command<COMMAND_SET_CAR_STRONG>(CPools::GetVehicleRef(car), true);// авто устойчивое - да.
				return 0;
			}
			if (value == 0) {
				Command<COMMAND_SET_CAR_STRONG>(CPools::GetVehicleRef(car), false);// авто устойчивым - нет.
				return 0;
			}
		}
		else { throw "bad argument in function setcarstrong"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int putincar(lua_State* L) {// переместить педа в авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {// указатели на педа и авто.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);//  получить указатель на педа.

			const void* p1 = lua_topointer(L, 2);
			CVehicle* car = findcarinpool(p1);//  получить указатель на авто.

			float x = car->GetPosition().x; // отправить в стек.
			float y = car->GetPosition().y; // отправить в стек.
			float z = car->GetPosition().z; // отправить в стек.
			float angle = car->GetHeading() * 57.295779f;// получить угол авто. новый SDK возвращает радианы, переводим в градусы для опкода.
			ped->SetObjective(OBJECTIVE_ENTER_CAR_AS_DRIVER, car);
			ped->WarpPedIntoCar(car); // переместить педа в авто
			this_thread::sleep_for(chrono::milliseconds(20));
			Command<COMMAND_SET_CAR_COORDINATES>(CPools::GetVehicleRef(car), x, y, z);// установить координаты авто.

			Command<COMMAND_SET_CAR_HEADING>(CPools::GetVehicleRef(car), angle);

			//CPed* player = FindPlayerPed();// найти педа
			//if (ped != player) {
			//	Command<COMMAND_WARP_CHAR_INTO_CAR>(CPools::GetPedRef(ped), CPools::GetVehicleRef(car));
			//	return 0;
			//}
			//else {
			//	Command<COMMAND_WARP_PLAYER_INTO_CAR>(CWorld::PlayerInFocus, CPools::GetVehicleRef(car));}
			return 0;

		}
		else { throw "bad argument in function putincar"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_attack_car(lua_State* L) {// пед атакует авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {// указатели на педа и авто.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.

			const void* p1 = lua_topointer(L, 2);
			CVehicle* car = findcarinpool(p1);//  получить указатель на авто.
			//Command<COMMAND_SET_CHAR_OBJ_DESTROY_CAR>(CPools::GetPedRef(ped), CPools::GetVehicleRef(car));
			ped->SetObjective(OBJECTIVE_DESTROY_CAR, car); //уничтожить машину 01D9
			return 0;

		}
		else { throw "bad argument in function ped_attack_car"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int checkcheat(lua_State* L) {//чит код введен?.
	try {
		if (LUA_TSTRING == lua_type(L, 1)) {// если строка
			string cheat = lua_tostring(L, 1);//
			if (-1 != cheatstr.find(cheat)) {
				cheatstr.clear();
				lua_pushboolean(L, true); cheatstr.erase(0, 1);
				return 1;
			}
			else {
				lua_pushboolean(L, false);
				return 1;
			}
		}
		else { throw "bad argument in function getpedarmour option of the player"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
}


int star_timer(lua_State* L) {
	try {
		if (LUA_TSTRING == lua_type(L, 1)) {// если число
			const char* str = lua_tostring(L, 1);// строка.
			Command<COMMAND_DISPLAY_ONSCREEN_TIMER_WITH_STRING>(str);
			lua_pushstring(L, str);
			return 1;
		}
		else { throw "bad argument in function  star_timer"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int stop_timer(lua_State* L) {// остановить таймер.
	try {
		if (LUA_TSTRING == lua_type(L, 1)) {// если число
			const char* str = lua_tostring(L, 1);// строка.
			static int timer = 13608;
			CUserDisplay::OnscnTimer.ClearClock(timer); //014F: stop_timer $3402
			Command<COMMAND_CLEAR_ONSCREEN_TIMER>(str);
			var_$3402 = 0;
			return 0;
		}
		else { throw "bad argument in function stop_timer"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int timer_donw(lua_State* L) {// таймер на уменьшение.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TSTRING == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {
			int time = lua_tointeger(L, 1);// если число
			const char* str1 = lua_tostring(L, 2);// строка.
			char* str = (char*)str1;
			var_$3402 = 1000 * time; //in CLEO $3402 = 120000

			int tipe = lua_tointeger(L, 3);// если число
			static int timer = 13608;
			CUserDisplay::OnscnTimer.AddClock(timer, str, tipe);
			return 0;
		}
		else { throw "bad argument in function timer_donw"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_frozen(lua_State* L) {// заморозить игpока.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {
			int status = lua_tointeger(L, 1);// если число

			Command<COMMAND_SET_PLAYER_CONTROL>(CWorld::PlayerInFocus, status);

			return 0;
		}
		else { throw "bad argument in ped_frozen"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int hold_cellphone(lua_State* L) {// поднять телефон.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);

			CPed* ped = findpedinpool(p);//  получить указатель на авто.
			int status = lua_tointeger(L, 2);// если число.
			Command<COMMAND_SET_CHAR_ANSWERING_MOBILE>(ped, status);
			return 0;
		}
		else { throw "bad argument in function hold_cellphone"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int car_lastweapondamage(lua_State* L) {// номер оружие, которое нанесло урон авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {//число.
			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);//  получить указатель на авто.
			unsigned char c = car->m_nLastWeaponDamage;

			int d = (int)c;
			lua_pushinteger(L, d);
			return 1;
		}
		else { throw "bad argument in function car_lastweapondamage"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int car_currentgear(lua_State* L) {// текущая передача авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.

			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.

			unsigned char c = car->m_nCurrentGear;
			int d = (int)c;
			lua_pushinteger(L, d);
			return 1;
		}
		else { throw "bad argument in function car_currentgear"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int getcar_model(lua_State* L) {// получить модель авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {//число.

			const void* p = lua_topointer(L, -1);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.

			unsigned char c = car->m_nModelIndex;
			int d = (int)c;
			lua_pushinteger(L, d);
			return 1;
		}
		else { throw "bad argument in function car_currentgear"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int setcarsiren(lua_State* L) {// установить сирену для авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {

			const void* p = lua_topointer(L, -1);

			CVehicle* car = findcarinpool(p);//  получить указатель на авто.

			int tipe = lua_tointeger(L, 2);
			car->m_nSirenOrAlarm = tipe;
			//car->m_bSirenOrAlarm = tipe;
			return 0;
		}
		else { throw "bad argument in function setcarsiren"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_car_as_passenger(lua_State* L) {// пед садится в авто как пассажир.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {//число.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);//  получить указатель на педа.

			const void* p1 = lua_topointer(L, 2);
			CVehicle* car = findcarinpool(p1);//  получить указатель на авто.

			ped->SetObjective(OBJECTIVE_ENTER_CAR_AS_PASSENGER, car); //сьесть в машину. 01D9
			return 0;

		}
		else { throw "bad argument in function ped_car_as_passenger"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_car_as_driver(lua_State* L) {// пед садится в авто как водитель.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {//число.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.

			const void* p1 = lua_topointer(L, 2);

			CVehicle* car = findcarinpool(p1);//  получить указатель на авто.
			ped->SetObjective(OBJECTIVE_ENTER_CAR_AS_DRIVER, car); //пед садится в авто как водитель.
			return 0;
		}
		else { throw "bad argument in function ped_car_as_driver"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int camera_at_point(lua_State* L) {//переместить камеру в координатах.
	try {
		if (LUA_TNUMBER == lua_type(L, -4) && LUA_TNUMBER == lua_type(L, -3) &&
			LUA_TNUMBER == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {
			float x = lua_tonumber(L, 1);  float y = lua_tonumber(L, 2);
			float z = lua_tonumber(L, 3);  int tipe = lua_tointeger(L, 4);
			CVector pos = { x, y, z };

			Command<COMMAND_POINT_CAMERA_AT_POINT>(pos.x, pos.y, pos.z, tipe); //
/*			CCamera* cam;
			cam = &TheCamera;// камера из SDK (раньше хардкод 0x7E4688).
			cam->TakeControlNoEntity(pos, 1, tipe); */  //  POINT_CAMERA_AT_POINT
			return 0;
		}
		else { throw "bad argument in function camera_at_point"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_camera_position(lua_State* L) {//установить камеру в координатах.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)
			&& LUA_TNUMBER == lua_type(L, 4) && LUA_TNUMBER == lua_type(L, 5) && LUA_TNUMBER == lua_type(L, 6)) {
			float x = lua_tonumber(L, 1);  float y = lua_tonumber(L, 2);
			float z = lua_tonumber(L, 3);  float rx = lua_tonumber(L, 4);
			float ry = lua_tonumber(L, 5); float rz = lua_tonumber(L, 6);
			CVector pos = { x, y, z };

			CVector r = { rx, ry, rz };
			//Command<COMMAND_SET_FIXED_CAMERA_POSITION>(pos.x, pos.y, pos.z, rx, ry, rz); //
			CCamera* cam;
			cam = &TheCamera;// камера из SDK (раньше хардкод 0x7E4688).
			cam->SetCamPositionForFixedMode(pos, r); // SET_FIXED_CAMERA_POSITION opcode_015F
			return 0;
		}
		else { throw "bad argument in function set_camera_position"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_camera_and_point(lua_State* L) {// установить и переместить камеру в координатах.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)
			&& LUA_TNUMBER == lua_type(L, 4) && LUA_TNUMBER == lua_type(L, 5) && LUA_TNUMBER == lua_type(L, 6)) {
			float x = lua_tonumber(L, 1);  float y = lua_tonumber(L, 2);
			float z = lua_tonumber(L, 3);
			float x1 = lua_tonumber(L, 4);  float y1 = lua_tonumber(L, 5);
			float z1 = lua_tonumber(L, 6);

			CVector pos = { x, y, z };
			CVector pos1 = { x1, y1, z1 };
			CVector r = { 0.0, 0.0, 0.0 };
			CCamera* cam;
			cam = &TheCamera;// камера из SDK (раньше хардкод 0x7E4688).
			cam->SetCamPositionForFixedMode(pos, r); // SET_FIXED_CAMERA_POSITION opcode_015F
			//Command<COMMAND_POINT_CAMERA_AT_POINT>(pos.x, pos.y, pos.z, tipe); //
			cam->TakeControlNoEntity(pos1, 1, 1); // POINT_CAMERA_AT_POINT
			return 0;
		}
		else { throw "bad argument in function set_camera_and_point"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

static int getcord(queue<float>q, const void* p) {
	CVehicle* car = findcarinpool(p);//  получить указатель на авто.
	while (!q.empty()) {
		this_thread::sleep_for(chrono::milliseconds(1));
		float x = q.front(); q.pop();
		float y = q.front(); q.pop();
		float z = q.front(); q.pop();

		Command<COMMAND_CAR_GOTO_COORDINATES>(car, x, y, z);// авто едет на координаты.
		while (!car->IsSphereTouchingVehicle(x, y, z, 3.0)) {
			this_thread::sleep_for(chrono::milliseconds(1));
			//if (car->m_fHealth < 100){
			//	break;
			//}
		};
	};
	return 0;
};
void writelog1(float x) {// запись ошибок в файл.
	string path = "queqe.txt";
	fstream f1; {f1.open(path, fstream::in | fstream::out | fstream::app);
	f1 << x;  f1 << "\n"; }
	f1.close();
};

void writelog1(int x) {// запись ошибок в файл.
	string path = "queqe.txt";
	fstream f1; {f1.open(path, fstream::in | fstream::out | fstream::app);
	f1 << x;  f1 << "\n"; }
	f1.close();
};
int go_to_route(lua_State* L) {//установить маршрут авто.
	try {
		if (LUA_TTABLE == lua_type(L, 2) && LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			lua_State* L1 = luaL_newstate();
			luaL_openlibs(L1);// открыть допю. библиотеки.
			int stacksize = lua_gettop(L);// кол-во элементов в  стек.
			stacksize++;
			for (int i = 1; i < stacksize; i++) {
				lua_xmove(L, L1, 1);
			};
			if (LUA_TLIGHTUSERDATA == lua_type(L1, -1)) {
				const void* p = lua_topointer(L1, -1);
				lua_pop(L1, 1);	int counts = luaL_len(L1, 1);
				counts += 2; queue<float>q = {};// очередь для хранение всех значение координат из файла.
				for (int i = 1; i < counts; i++) {
					lua_pushinteger(L1, i);
					lua_gettable(L1, -2);
					if (LUA_TNUMBER == lua_type(L1, -1)) {
						float cord = lua_tonumber(L1, -1);
						q.push(cord); lua_pop(L1, 1);
					}
				};
				lua_close(L1);
				std::thread(getcord, q, p).detach();
				return 0;
			}
		}
		else { throw "bad argument in function go_to_route"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_current_name_luascript(lua_State* L) {// получить имя текущего lua файла.
	lua_pushlightuserdata(L, L);  /*отправить адрес, который является ключом в стек. */
	lua_gettable(L, LUA_REGISTRYINDEX);  /* получить таблицу и значение ключа будет в -1 */
	char const* luaname = lua_tostring(L, -1);//имя lua скрипта.
	lua_pushstring(L, luaname);
	return 1;
};

int restore_camera(lua_State* L) {// восстановить камеру.

	CCamera* cam;
	cam = &TheCamera;// камера из SDK (раньше хардкод 0x7E4688).
	cam->Restore();
	cam->RestoreWithJumpCut();
	//Command<COMMAND_RESTORE_CAMERA>();
	//Command<COMMAND_RESTORE_CAMERA_JUMPCUT>();
	return 0;
};

int ped_atack(lua_State* L) {// пед бьет.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {

			const void* p1 = lua_topointer(L, 1);

			CPed* p = findpedinpool(p1);//  получить указатель на педа.

			for (auto ped : CPools::ms_pPedPool) {
				if (ped == p) {
					//ped->Attack();
					this_thread::sleep_for(chrono::milliseconds(1));
					//spite::active(true, ped);
					//this_thread::sleep_for(chrono::milliseconds(1000));
					//unsigned int x =	ped->Attack();// правой рукой.
					return 0;
				}
			}
		}
		else { throw "bad argument in function ped_atack"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int flash_hud(lua_State* L) {// Мигание элементов HUD.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {
			int status = lua_tointeger(L, 1);// если число

			Command<COMMAND_FLASH_HUD_OBJECT>(status);
			//Мигание элементов HUD'а номер 8
			//	Номера элементов :
			//10 - полоса воздуха
			//	8 - радар
			//	4 - полоса здоровья
			//	"-1" - убрать мигание
			return 0;
		}
		else { throw "bad argument in function flash_hud"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_radio(lua_State* L) {// установить радио.
	try {//041E: set_radio_station 9 -1 .
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			int status = lua_tointeger(L, 1);// если число
			int type = lua_tointeger(L, 2);// если число
			//Указатель радиостанции - 1 = не начинать с начала файла станции, 0 = начинать с начала
			//Этот код операции устанавливает текущую радиостанцию ​​для воспроизведения в автомобиле, который поддерживает эту станцию.
			//В vc индекс станции тесно связан со списком радиостанций, за исключением индекса 9. Если используется индекс 9,
			// станция устанавливает себя на 10, предотвращая воспроизведение станции MP3 с этим кодом операции.
			Command<COMMAND_SET_RADIO_CHANNEL>(status, type);
			return 0;
		}
		else { throw "bad argument in function set_radio"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_car_tires(lua_State* L) {// проколоть шину.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {//строка.

			const void* p1 = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p1);//  получить указатель на авто.
			int wheel = lua_tointeger(L, 2);// номер колеса.
			Command<COMMAND_BURST_CAR_TYRE>(CPools::GetVehicleRef(car), wheel);
			return 0;
		}
		else { throw "bad argument in function set_car_tires"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_wheel_status(lua_State* L) {// получить состояния шины авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// первый указатель на авто.
			//второй параметр номер колесо.

			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);//  получить указатель на авто.
			int wheel = lua_tointeger(L, 2);

			if (car && car->m_nVehicleClass == VEHICLE_AUTOMOBILE) {
				CAutomobile* automobile = reinterpret_cast<CAutomobile*>(car);
				int status = automobile->m_carDamage.GetWheelStatus(wheel);
				return 1;
			}
		}
		else { throw "bad argument in function get_car_tires"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_wheel_status(lua_State* L) {// установить состояния шин авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)){
		// первый указатель на авто. второй параметр номер колесf. третий статус, 0 = починка.

			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);//  получить указатель на авто.
			int wheel = lua_tointeger(L, 2);
			int status = lua_tointeger(L, 3);
			if (car && car->m_nVehicleClass == VEHICLE_AUTOMOBILE) {
				CAutomobile* automobile = reinterpret_cast<CAutomobile*>(car);
				automobile->m_carDamage.SetWheelStatus(wheel, status);
				return 0;
			}
		}
		else { throw "bad argument in function set_car_tires"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_skin(lua_State* L) {// установить скин педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TSTRING == lua_type(L, 2)) {

			const void* p = lua_topointer(L, 1);

			CPed* ped = findpedinpool(p);//  получить указатель на педа.
			char const* model = lua_tostring(L, 2);// модель скина

			Command<COMMAND_LOAD_ALL_MODELS_NOW>(false);
			Command<COMMAND_UNDRESS_CHAR>(CPools::GetPedRef(ped), model);
			Command<COMMAND_LOAD_ALL_MODELS_NOW>(false);
			Command<COMMAND_DRESS_CHAR>(CPools::GetPedRef(ped));
			this_thread::sleep_for(chrono::milliseconds(1));
			return 0;
		}
		else { throw "bad argument in function ped_atack"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int del_anim(lua_State* L) {// удалить анимацию.
	try {// CAnimManager::BlendAnimation(PlayerPed->rwObject, 0, 14, 10000.0f);.
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) ) {//число.

			const void* p = lua_topointer(L, 1);
			int tipe = 0;
			int idanimation = 3;
			int time = 100;
			CPed* ped = findpedinpool(p);//  получить указатель на педа.
			RpClump* pe = ped->m_pRwClump;
			CAnimManager::BlendAnimation(pe, tipe, idanimation, time);
		}
		else { throw "bad argument in function del_anim"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int getcarspeed(lua_State* L) {// получить скорость авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			float speed;// переменная хранить скорость авто.
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			if (car == NULL) { lua_pushnumber(L, 0); return 1; }
			//speed = car->m_fTotSpeed;
			Command<COMMAND_GET_CAR_SPEED>(CPools::GetVehicleRef(car), &speed);// получить скорость авто.
			lua_pushnumber(L, speed);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function getcarspeed"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int remove_all_weapons_ped(lua_State* L) {// удалить все оружия педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, -1)) {// указатель на педа.
			const void* p = lua_topointer(L, -1);

			CPed* ped = findpedinpool(p);// получить указатель на педа.
			ped->ClearWeapons();
			return 0;
		}
		else { throw "bad argument in function remove_all_weapons_ped"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int Getweaponslot(lua_State* L) {// получить номер слота по типу оружия.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {//число.

			const void* p = lua_topointer(L, 1);
			int weapontype = lua_tointeger(L, 2);
			CPed* ped = findpedinpool(p);//  получить указатель на педа.
			eWeaponType w = (eWeaponType)weapontype;
			int shot = ped->GetWeaponSlot(w);// номер слота оружие.
			lua_pushinteger(L, shot);
			return 1;
		}
		else { throw "bad argument in function Getweaponslot"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_damage_weapon_ped(lua_State* L) {// получить последний урон педа от оружия.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {//число.

			const void* p = lua_topointer(L, 1);

			CPed* ped = findpedinpool(p);//  получить указатель на педа.
			int damageweapon = ped->m_nLastDamWep;
			lua_pushinteger(L, damageweapon);
			return 1;
		}
		else { throw "bad argument in function get_damage_weapon_ped"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_aimgun_ped(lua_State* L) {// получить показатель цели педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {//число.

			const void* p = lua_topointer(L, 1);

			CPed* ped = findpedinpool(p);//  получить указатель на педа.
			int aimgun = ped->AimGun();
			lua_pushinteger(L, aimgun);
			return 1;
		}
		else { throw "bad argument in function get_aimgun_ped"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_ammo_weapon_ped(lua_State* L) {// получить кол-во патроны текущего оружие педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {//число.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);//  получить указатель на педа.

			CWeapon weapon = ped->m_aWeapons[ped->m_nCurrentWeapon];
			int ammo = weapon.m_nAmmoTotal; // кол-во патроны текущего оружие.
			lua_pushinteger(L, ammo);
			return 1;
		}
		else { throw "bad argument in function get_ammo_weapon_ped"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int createfireonped(lua_State* L) {// создать огонь на педе.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			int fire;// переменная хранить id огня.
			CPed* ped = findpedinpool(p);//  получить указатель на педа.
			Command<COMMAND_START_CHAR_FIRE>(CPools::GetPedRef(ped), &fire);// создать огонь на педе.
			firesids.emplace(fire, L); // добавить id огня в map.

			lua_pushinteger(L, fire);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function createfireonped"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int createfireoncar(lua_State* L) {// создать огонь на авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			int fire;// переменная хранить id огоня.

			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			Command<COMMAND_START_CAR_FIRE>(CPools::GetVehicleRef(car), &fire);// создать огонь на авто.
			firesids.emplace(fire, L); // добавить id огня в map.
			lua_pushinteger(L, fire);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function createfireoncar"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int createfireoncords(lua_State* L) {// создать огонь на координатах.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {
			float x = lua_tonumber(L, 1);
			float y = lua_tonumber(L, 2); float z = lua_tonumber(L, 3);
			int fire;// переменная хранить id огоня.
			CVector pos = { x,y,z };

			Command<COMMAND_START_SCRIPT_FIRE>(pos.x, pos.y, pos.z, &fire);// создать огонь на координатах.
			firesids.emplace(fire, L); // id map
			lua_pushinteger(L, fire);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function createfireoncords"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int remove_fire(lua_State* L) {// удалить огонь.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// значение id огня.
			int fire = lua_tointeger(L, 1);
			Command<COMMAND_REMOVE_SCRIPT_FIRE>(fire);// удалить огонь.
			return 0;
		}
		else { throw "bad argument in function remove_fire"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_shutdown(lua_State* L) {// отключение текущего оружие педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {//число.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);//  получить указатель на педа.

			CWeapon weapon = ped->m_aWeapons[ped->m_nCurrentWeapon];// получить оружие текущего слота.
			//weapon.InitialiseWeapons();
			weapon.Shutdown();
			//weapon.ShutdownWeapons();
			//weapon.Reload();
			//lua_pushinteger(L, ammo);
			return 0;
		}
		else { throw "bad argument in function ped_shutdown"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int is_ped_damage_from_ped(lua_State* L) {// получил ли пед урон от педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {//указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);//  получить указатель на педа.
			const void* p1 = lua_topointer(L, 2);
			CPed* ped1 = findpedinpool(p1);//  получить указатель на педа.
			bool check_damage = Command<COMMAND_HAS_CHAR_BEEN_DAMAGED_BY_CHAR>(CPools::GetPedRef(ped), CPools::GetPedRef(ped1));
			lua_pushboolean(L, check_damage);
			return 1;
		}
		else { throw "bad argument in function is_ped_damage_from_ped"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int is_ped_passenger_car(lua_State* L) {// пед пассажир авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {//указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);//  получить указатель на педа.

			const void* p1 = lua_topointer(L, 2);
			CVehicle* car = findcarinpool(p1);//  получить указатель на авто.
			bool check_passenger = car->IsPassenger(ped);

			lua_pushboolean(L, check_passenger);
			return 1;
		}
		else { throw "bad argument in function is_ped_passenger_car"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int is_ped_driver_car(lua_State* L) {// пед водитель авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {//указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);//  получить указатель на педа.

			const void* p1 = lua_topointer(L, 2);
			CVehicle* car = findcarinpool(p1);//  получить указатель на авто.
			bool check_driver = car->IsDriver(ped); // проверить водитель авто?

			lua_pushboolean(L, check_driver);
			return 1;
		}
		else { throw "bad argument in function is_ped_passenger_car"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int is_targetting_in_ped(lua_State* L) {// игрок целиться в педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {//указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);//  получить указатель на педа.
			bool check_target = Command<COMMAND_IS_PLAYER_TARGETTING_CHAR>(CWorld::PlayerInFocus, CPools::GetPedRef(ped));
			lua_pushboolean(L, check_target);
			return 1;
		}
		else { throw "bad argument in function is_targetting_in_ped"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int Remove_weapon_model(lua_State* L) {// удалить оружие у педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {//число.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);//  получить указатель на педа.
			int idweapon = lua_tointeger(L, 2);

			int tipe = find_model_in_map(model_and_type, idweapon);// тип оружие.

			ped->SetAmmo((eWeaponType)tipe, 0);// обнулить кол-во патронов.
			CWeapon weapon = ped->m_aWeapons[ped->m_nCurrentWeapon];// получить оружие из текущего слота.
			ped->RemoveWeaponModel(idweapon); // удалить модель оружие.
			weapon.Shutdown();// отключить оружие.
			weapon.UpdateWeapons();// обновить оружия.
			return 0;
		}
		else { throw "bad argument in function Remove_weapon_model"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int Createped(lua_State* L) {// макрос создать педа.
	try {
		if (LUA_TSTRING == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {// значение число.
			const char* name_ped = lua_tostring(L, 1);

			int model = find_in_map(name_peds_list, name_ped);// модель педа.
			int type = find_in_map(type_peds_list, name_ped);// тип педа.

			float x = lua_tonumber(L, 2); float y = lua_tonumber(L, 3); float z = lua_tonumber(L, 4);
			CVector pos = { x, y, z };
			CPed* ped = NULL;

			if (model != 0) {// обычный пед из списка name_peds_list.
				load_model_before_avalible(model);
				//CWorld::Players[CWorld::PlayerInFocus].m_nMoney += type;// дать денег
				Command<COMMAND_CREATE_CHAR>(type, model, pos.x, pos.y, pos.z, &ped);
				Command<COMMAND_MARK_MODEL_AS_NO_LONGER_NEEDED>(model);
			}
			else {// имени нет в обычных списках → это спец-актор (IGMERC, IGPHIL ...).
				int spectype = find_in_map(type_specpeds_list, name_ped);// пол спец-актора: 4 мужской, 5 женский.
				if (spectype != 0) {// создать мужского или женского педа и дать ему скин спец-актора.
					const char* base_ped = (spectype == PED_TYPE_CIVFEMALE) ? "WFYST" : "HMYBE";// базовая модель по полу.
					int base_model = find_in_map(name_peds_list, base_ped);// модель базового педа.
					int base_type = find_in_map(type_peds_list, base_ped);// тип базового педа.
					load_model_before_avalible(base_model);
					Command<COMMAND_CREATE_CHAR>(base_type, base_model, pos.x, pos.y, pos.z, &ped);
					Command<COMMAND_MARK_MODEL_AS_NO_LONGER_NEEDED>(base_model);
					if (ped != NULL) {// надеть скин спец-актора на созданного педа.
						CPed* p = findpedinpool(ped);// получить указатель на педа.
						Command<COMMAND_LOAD_ALL_MODELS_NOW>(false);
						Command<COMMAND_UNDRESS_CHAR>(CPools::GetPedRef(p), name_ped);// скин спец-актора.
						Command<COMMAND_LOAD_ALL_MODELS_NOW>(false);
						Command<COMMAND_DRESS_CHAR>(CPools::GetPedRef(p));
					}
				}
			}

			if (ped == NULL) { writelog("Createped: CREATE_CHAR вернул NULL"); lua_pushnil(L); return 1; }
			mappeds.emplace(ped, L);// добавить map для педов.
			lua_pushlightuserdata(L, ped);// отправить в стек и получить из стека можно.
			return 1;
		}// int

		else { throw "bad argument in function Createped"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};
int expectations(int model, CVehicle* car) {
	if (car == NULL) { writelog("Createcar: EXPECTATIONS: car is NULL"); return 1; }
	while (true) {
		this_thread::sleep_for(chrono::milliseconds(10));// задержка
		if (car->IsVisible()) {

			Command<COMMAND_MARK_MODEL_AS_NO_LONGER_NEEDED>(model);
			break;
		}
	};
	return 0;
};

int Createcar(lua_State* L) {// макрос создать авто на координатах.
	try {
		if (LUA_TSTRING == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)
			&& LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {// значение число.
			CVehicle* car = NULL;
			const char* name_model = lua_tostring(L, 1);// модель авто.

			int model = find_in_map(car_model_list, name_model);
			float x = lua_tonumber(L, 2); float y = lua_tonumber(L, 3);
			float z = lua_tonumber(L, 4); CVector pos = { x, y, z };
			load_model_before_avalible(model); // загрузить модель полносттью.

   			Command<COMMAND_CREATE_CAR>(model, pos.x, pos.y, pos.z, &car);
			if (car == NULL) {
				writelog("Createcar: CREATE_CAR вернул NULL (модель не создана)");
				lua_pushnil(L);
				return 1;
			}
			cpp_tracef("Createcar: создана %s (id=%d) car=%p", name_model, model, (void*)car);
			Command<COMMAND_MARK_MODEL_AS_NO_LONGER_NEEDED>(model);
			cpp_tracef("Createcar: MARK no-longer-needed id=%d OK", model);

			car->m_eDoorLock = (eDoorLock)1;
			mapcars.emplace(car, L);// добавить в map для авто.
			lua_pushlightuserdata(L, car);// отправить в стек указатель на авто.
			return 1;
		}// int

		else { throw "bad argument in function Createcar"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int Createobj(lua_State* L) {// макрос создать объект.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)
			&& LUA_TNUMBER == lua_type(L, 4)) {// значение число.
			int model = lua_tointeger(L, 1);// модель авто.
			float x = lua_tonumber(L, 2); float y = lua_tonumber(L, 3);
			float z = lua_tonumber(L, 4); CVector pos = { x, y, z };
			CObject* obj = NULL;
			load_model_before_avalible(model); // загрузить модель полносттью.
			Command<COMMAND_CREATE_OBJECT>(model, pos.x, pos.y, pos.z, &obj);
			if (obj == NULL) { writelog("Createobj: CREATE_OBJECT вернул NULL"); lua_pushnil(L); return 1; }
			this_thread::sleep_for(chrono::milliseconds(20));// задержка
			Command<COMMAND_MARK_MODEL_AS_NO_LONGER_NEEDED>(model);
			mapobjs.emplace(obj, L);// добавить в map для авто.
			lua_pushlightuserdata(L, obj);// отправить в стек указатель на объект.
			return 1;
		}// int

		else { throw "bad argument in function Createobj"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};
//--function Giveweaponped(ped, ammo, ...)--дать педу оружие и патроны.
void ped_weapon_give(CPed* ped, int typemodel, int ammo) {
	CPed* player = FindPlayerPed();// найти педа
	if (ped == player) { Command<COMMAND_GIVE_WEAPON_TO_PLAYER>(CWorld::PlayerInFocus, typemodel, ammo); }
	else {
		Command<COMMAND_GIVE_WEAPON_TO_CHAR>(CPools::GetPedRef(ped), typemodel, ammo);// Дать оружие педу.
	}
};

int Giveweaponped(lua_State* L) {// макрос дать педу оружие и патроны.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TSTRING == lua_type(L, 3)) {

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);//  получить указатель на педа.
			int ammo = lua_tointeger(L, 2);
			lua_remove(L, 2); lua_remove(L, 1);
			int size = lua_gettop(L); size++;

			for (int i = 1; i < size; i++) {
				const char* name_weapon = lua_tostring(L, i);
				int model = find_in_map(name_weapon_list, name_weapon);// модель оружие
				load_model_before_avalible(model); // загрузить модель полносттью.
				int type = find_in_map(types_weapon_list, name_weapon);// тип оружие.
				ped_weapon_give(ped, type, ammo);
				this_thread::sleep_for(chrono::milliseconds(1));// задержка
				Command<COMMAND_MARK_MODEL_AS_NO_LONGER_NEEDED>(model);
			}

			lua_pop(L, lua_gettop(L));// удалить n элементы из стека.
			return 0;
		}
		else { throw "bad argument in function Giveweaponped"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

//--function Create_weapon_pickup(m, typepickup, ammo, x, y, z)  --создать пикап оружие.
int Create_weapon_pickup(lua_State* L) {// макрос создать пикап оружие.
	try {
		if (LUA_TSTRING == lua_type(L, 1) || LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)
			&& LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4) &&
			LUA_TNUMBER == lua_type(L, 5) && LUA_TNUMBER == lua_type(L, 6)) {

			int typepick = lua_tointeger(L, 2);// тип пикапа.
			int ammo = lua_tointeger(L, 3); // начальное кол-во патронов в оружие.
			float x = lua_tonumber(L, 4);
			float y = lua_tonumber(L, 5); float z = lua_tonumber(L, 6);
			int idpickup;// переменная, которая хранить id пикапа.
			CVector pos = { x, y, z };	int model;
			if (LUA_TSTRING == lua_type(L, 1)) {
				const char* name_weapon = lua_tostring(L, 1); // имя оружие если строка.
				model = find_in_map(name_weapon_list, name_weapon);// модель оружие
			}
			else {
				if (LUA_TNUMBER == lua_type(L, 1)) { model = lua_tointeger(L, 1); }
			}
			load_model_before_avalible(model); // загрузить модель полносттью.
			Command<COMMAND_CREATE_PICKUP_WITH_AMMO>(model, typepick, ammo, pos.x, pos.y, pos.z, &idpickup);
			pickupsids.emplace(idpickup, L);// добавить в map пикапов.
			lua_pushinteger(L, idpickup);// отправить id пикапа в стек.
			Command<COMMAND_MARK_MODEL_AS_NO_LONGER_NEEDED>(model);
			return 1;
		}
		else { throw "bad argument in function Create_weapon_pickup"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int expectations2(int model) {
		this_thread::sleep_for(chrono::milliseconds(1000));// задержка

			Command<COMMAND_MARK_MODEL_AS_NO_LONGER_NEEDED>(model);

	return 0;
};

//function Create_pickup(model, typepickup, x, y, z) --создать пикап.

int Create_pickup(lua_State* L) {// макрос создать пикап.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)
			&& LUA_TNUMBER == lua_type(L, 4) && LUA_TNUMBER == lua_type(L, 5)) {
			int model = lua_tointeger(L, 1); // модель пикапа.

			load_model_before_avalible(model); // загрузить модель полносттью.
			int type = lua_tointeger(L, 2);// тип пикапа.
			float x = lua_tonumber(L, 3);	float y = lua_tonumber(L, 4);
			float z = lua_tonumber(L, 5);// координаты.

			int idpickup;// переменная, которая хранить id пикапа.
			CVector pos = { x, y, z };
			Command<COMMAND_CREATE_PICKUP>(model, type, pos.x, pos.y, pos.z, &idpickup);
			//this_thread::sleep_for(chrono::milliseconds(1));// задержка
			pickupsids.emplace(idpickup, L);// добавить в map пикапов.
			lua_pushinteger(L, idpickup);// отправить id пикапа в стек.

			//std::thread(expectations2, model).detach();
//			Command<COMMAND_MARK_MODEL_AS_NO_LONGER_NEEDED>(model);
			return 1;
		}
		else { throw "bad argument in function Create_pickup"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int Get_model_and_type_current_weapon_ped(lua_State* L) {// макрос получить модель и тип текущего оружие педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {//число.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);//  получить указатель на педа.

			CWeapon weapon = ped->m_aWeapons[ped->m_nCurrentWeapon];
			eWeaponType type = weapon.m_eWeaponType;
			CWeaponInfo* winfo = CWeaponInfo::GetWeaponInfo(type);

			int idweapon = winfo->m_nModelId;

			lua_pushinteger(L, idweapon);
			lua_pushinteger(L, type);
			return 2;
		}
		else { throw "bad argument in function Get_model_and_type_current_weapon_ped"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int is_ped_in_car(lua_State* L) {// игрок в авто?
 	try {
 		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.

 			const void* p = lua_topointer(L, 1);
 			CPed* ped = findpedinpool(p);// получить указатель на педа.
 			if (ped == NULL) { lua_pushboolean(L, false); return 1; }
 			if (ped->m_bInVehicle && ped->m_pVehicle != NULL) {// в авто пед?
 				cpp_tracef("is_ped_in_car: пед=%p В МАШИНЕ (m_pVehicle=%p)", ped, (void*)ped->m_pVehicle);
 				lua_pushboolean(L, true);
 				return 1;
 			}
 			else {
 				lua_pushboolean(L, false);
 				return 1;// получить указатель на хенлд авто в котором сидит пед.
 			}
 		}
 		else { throw "bad argument in function is_ped_in_car"; }
 	}
 	catch (const char* x) { writelog(x); }// записать ошибку в файл.
 };

int ped_car(lua_State* L) {// авто педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			if (ped == NULL) { lua_pushlightuserdata(L, (CVehicle*)NULL); return 1; }
			if (ped->m_bInVehicle && ped->m_pVehicle != NULL) {// в авто пед?
				CVehicle* car = ped->m_pVehicle;
				lua_pushlightuserdata(L, car);// отправить в стек true и указатель на авто.
				return 1;
			}
			else {
				CVehicle* car = NULL;//если пед не в авто вернуть null;
				lua_pushlightuserdata(L, car);// отправить в стек и получить из стека можно
				return 1;// получить указатель на null.
			}
		}
		else { throw "bad argument in function ped_car"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int get_model_current_weapon_ped(lua_State* L) {// макрос получить модель текущего оружие педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {//число.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);//  получить указатель на педа.

			CWeapon weapon = ped->m_aWeapons[ped->m_nCurrentWeapon];
			eWeaponType type = weapon.m_eWeaponType;
			CWeaponInfo* winfo = CWeaponInfo::GetWeaponInfo(type);
			int idweapon = winfo->m_nModelId;

			lua_pushinteger(L, idweapon);
			return 1;
		}
		else { throw "bad argument in function get_model_current_weapon_ped"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};
int get_type_current_weapon_ped(lua_State* L) {// макрос получить тип текущего оружие педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {//число.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);//  получить указатель на педа.

			CWeapon weapon = ped->m_aWeapons[ped->m_nCurrentWeapon];
			int type = weapon.m_eWeaponType;
			lua_pushinteger(L, type);
			return 1;
		}
		else { throw "bad argument in function get_type_current_weapon_ped"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_damage_current_weapon(lua_State* L) {// получить уровень урона текущего оружие.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {//число.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);//  получить указатель на педа.

			CWeapon weapon = ped->m_aWeapons[ped->m_nCurrentWeapon];
			eWeaponType type = weapon.m_eWeaponType;
			CWeaponInfo* w = CWeaponInfo::GetWeaponInfo(type);
			int damage = w->m_nDamage;
			lua_pushinteger(L, damage);
			return 1;
		}
		else { throw "bad argument in function get_damage_current_weapon"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_damage_current_weapon(lua_State* L) {// установить уровень урона текущего оружие педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {//число.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);//  получить указатель на педа.
			int damage = lua_tointeger(L, 2);
			CWeapon weapon = ped->m_aWeapons[ped->m_nCurrentWeapon];

			eWeaponType type = weapon.m_eWeaponType;
			CWeaponInfo* w = CWeaponInfo::GetWeaponInfo(type);
			w->m_nDamage = damage;
			return 0;
		}
		else { throw "bad argument in function set_damage_current_weapon"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_clip_current_weapon(lua_State* L) {// установить объём обоймы текущего оружия педа (только для огнестрельного).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {//число.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);//  получить указатель на педа.
			int clip = lua_tointeger(L, 2);// объём обоймы.

			CWeapon weapon = ped->m_aWeapons[ped->m_nCurrentWeapon];
			eWeaponType type = weapon.m_eWeaponType;
			CWeaponInfo* w = CWeaponInfo::GetWeaponInfo(type);

			// Обойма есть только у огнестрельного оружия: пистолеты, ПП, пистолеты-пулемёты,
			// дробовики, винтовки, снайперки - у всех тип стрельбы WEAPON_FIRE_INSTANT_HIT.
			// MELEE (нож, бита) и PROJECTILE (гранаты, ракеты) в обойме не нуждаются.
			if (w->m_eWeaponFire != WEAPON_FIRE_INSTANT_HIT)
				throw "current weapon is not a firearm in set_clip_current_weapon";

			w->m_nAmountofAmmunition = clip;

			// Сразу наполнить обойму новым объёмом на текущей машине-педе,
			// чтобы HUD показывал clip/ammo корректно без перезарядки.
			CWeapon* cur = &ped->m_aWeapons[ped->m_nCurrentWeapon];
			if (cur->m_nAmmoTotal >= (unsigned)clip)
				cur->m_nAmmoInClip = clip;
			else
				cur->m_nAmmoInClip = cur->m_nAmmoTotal;

			return 0;
		}
		else { throw "bad argument in function set_clip_current_weapon"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_ped_state(lua_State* L) {// получить состояние педа (0-71, см. ePedState).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			int state = ped->m_ePedState;// текущее состояние педа.
			lua_pushinteger(L, state);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_ped_state"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_ped_last_state(lua_State* L) {// получить прошлое состояние педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			int state = ped->m_eLastPedState;// прошлое состояние педа.
			lua_pushinteger(L, state);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_ped_last_state"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_ped_state(lua_State* L) {// установить состояние педа (число из ePedState).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на педа и число.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			int state = lua_tointeger(L, 2);// новое состояние педа.
			ped->m_ePedState = (ePedState)state;// установить состояние.
			return 0;
		}
		else { throw "bad argument in function set_ped_state"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_ped_heading(lua_State* L) {// получить текущий угол поворота педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			float heading = ped->m_fHeadingCurrent;// текущий угол педа.
			lua_pushnumber(L, heading);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_ped_heading"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_ped_heading_goal(lua_State* L) {// установить желаемый угол поворота педа (пед доворачивается сам).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на педа и число.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			float angle = lua_tonumber(L, 2);// желаемый угол педа.
			ped->m_fHeadingGoal = angle;// установить желаемый угол.
			return 0;
		}
		else { throw "bad argument in function set_ped_heading_goal"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_ped_heading_goal(lua_State* L) {// получить желаемый угол поворота педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			float angle = ped->m_fHeadingGoal;// желаемый угол педа.
			lua_pushnumber(L, angle);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_ped_heading_goal"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_ped_heading_rate(lua_State* L) {// установить скорость поворота педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на педа и число.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			float rate = lua_tonumber(L, 2);// скорость поворота.
			ped->m_fHeadingChangeRate = rate;// установить скорость поворота.
			return 0;
		}
		else { throw "bad argument in function set_ped_heading_rate"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_ped_heading_rate(lua_State* L) {// получить скорость поворота педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			float rate = ped->m_fHeadingChangeRate;// скорость поворота педа.
			lua_pushnumber(L, rate);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_ped_heading_rate"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_ped_enter_type(lua_State* L) {// получить тип посадки педа в авто (11 - со стороны пассажира, 15 - со стороны водителя).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			int type = ped->m_nEnterType;// тип посадки педа.
			lua_pushinteger(L, type);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_ped_enter_type"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_ped_from_vehicle(lua_State* L) {// получить авто из которого вышел пед.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			CVehicle* car = ped->m_pFromVehicle;// авто из которого вышел пед.
			if (car == NULL) { return 0; }// нет авто - ничего не отправлять.
			lua_pushlightuserdata(L, car);// отправить указатель на авто.
			return 1;
		}
		else { throw "bad argument in function get_ped_from_vehicle"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_can_be_deleted(lua_State* L) {// можно ли удалить педа из мира.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			bool can = ped->CanBeDeleted();// можно ли удалить педа.
			lua_pushboolean(L, can);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function ped_can_be_deleted"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_is_gang_member(lua_State* L) {// пед член банды.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			bool gang = ped->IsGangMember();// член банды или нет.
			lua_pushboolean(L, gang);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function ped_is_gang_member"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_is_player(lua_State* L) {// пед это игрок.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			bool player = ped->IsPlayer();// игрок или нет.
			lua_pushboolean(L, player);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function ped_is_player"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_is_in_control(lua_State* L) {// пед управляем (может сам действовать).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			bool control = ped->IsPedInControl();// управляем или нет.
			lua_pushboolean(L, control);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function ped_is_in_control"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_is_shootable(lua_State* L) {// в педа можно стрелять.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			bool shoot = ped->IsPedShootable();// можно стрелять или нет.
			lua_pushboolean(L, shoot);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function ped_is_shootable"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_is_pointer_valid(lua_State* L) {// указатель на педа валиден.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			bool valid = ped->IsPointerValid();// валиден или нет.
			lua_pushboolean(L, valid);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function ped_is_pointer_valid"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_set_dead(lua_State* L) {// пометить педа мертвым (без нанесения урона).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			ped->SetDead();// пометить мертвым.
			return 0;
		}
		else { throw "bad argument in function ped_set_dead"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_dress(lua_State* L) {// пед надевает одежду (переодеть игрока после смены одежды).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			ped->Dress();// переодеть педа.
			return 0;
		}
		else { throw "bad argument in function ped_dress"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_calculate_new_orientation(lua_State* L) {// пересчитать ориентацию педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			ped->CalculateNewOrientation();// пересчитать ориентацию.
			return 0;
		}
		else { throw "bad argument in function ped_calculate_new_orientation"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_set_move_anim(lua_State* L) {// задать педу анимацию движения (под стиль ходьбы).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			ped->SetMoveAnim();// задать анимацию движения.
			return 0;
		}
		else { throw "bad argument in function ped_set_move_anim"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_set_get_up(lua_State* L) {// пед встает с земли.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			ped->SetGetUp();// встать.
			return 0;
		}
		else { throw "bad argument in function ped_set_get_up"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_set_jump(lua_State* L) {// пед прыгает.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			ped->SetJump();// прыгнуть.
			return 0;
		}
		else { throw "bad argument in function ped_set_jump"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_set_landing(lua_State* L) {// пед приземляется.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			ped->SetLanding();// приземлиться.
			return 0;
		}
		else { throw "bad argument in function ped_set_landing"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_set_idle(lua_State* L) {// пед переходит в состояние покоя.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			ped->SetIdle();// стоять на месте.
			return 0;
		}
		else { throw "bad argument in function ped_set_idle"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_fall(lua_State* L) {// пед падает.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			ped->Fall();// упасть.
			return 0;
		}
		else { throw "bad argument in function ped_fall"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_turn_body(lua_State* L) {// пед поворачивает корпус к цели.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			ped->TurnBody();// повернуть корпус.
			return 0;
		}
		else { throw "bad argument in function ped_turn_body"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_update_position(lua_State* L) {// обновить позицию педа по анимации.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			ped->UpdatePosition();// обновить позицию.
			return 0;
		}
		else { throw "bad argument in function ped_update_position"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_restore_heading_rate(lua_State* L) {// вернуть стандартную скорость поворота педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			ped->RestoreHeadingRate();// вернуть скорость поворота.
			return 0;
		}
		else { throw "bad argument in function ped_restore_heading_rate"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_restore_head_position(lua_State* L) {// вернуть голове педа стандартное положение.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			ped->RestoreHeadPosition();// вернуть положение головы.
			return 0;
		}
		else { throw "bad argument in function ped_restore_head_position"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_restore_previous_state(lua_State* L) {// вернуть педу прошлое состояние.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			ped->RestorePreviousState();// вернуть прошлое состояние.
			return 0;
		}
		else { throw "bad argument in function ped_restore_previous_state"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_restore_previous_objective(lua_State* L) {// вернуть педу прошлую цель.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			ped->RestorePreviousObjective();// вернуть прошлую цель.
			return 0;
		}
		else { throw "bad argument in function ped_restore_previous_objective"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_stop_non_partial_anims(lua_State* L) {// остановить базовые анимации педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			ped->StopNonPartialAnims();// остановить базовые анимации.
			return 0;
		}
		else { throw "bad argument in function ped_stop_non_partial_anims"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_restart_non_partial_anims(lua_State* L) {// возобновить базовые анимации педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			ped->RestartNonPartialAnims();// возобновить базовые анимации.
			return 0;
		}
		else { throw "bad argument in function ped_restart_non_partial_anims"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_clear_aim_flag(lua_State* L) {// сбросить у педа флаг прицеливания.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			ped->ClearAimFlag();// сбросить флаг прицеливания.
			return 0;
		}
		else { throw "bad argument in function ped_clear_aim_flag"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_clear_look_flag(lua_State* L) {// сбросить у педа флаг взгляда.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			ped->ClearLookFlag();// сбросить флаг взгляда.
			return 0;
		}
		else { throw "bad argument in function ped_clear_look_flag"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_clear_chat(lua_State* L) {// пед прекращает болтать.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			ped->ClearChat();// прекратить болтовню.
			return 0;
		}
		else { throw "bad argument in function ped_clear_chat"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_clear_attack(lua_State* L) {// сбросить у педы атаку.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			ped->ClearAttack();// сбросить атаку.
			return 0;
		}
		else { throw "bad argument in function ped_clear_attack"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_clear_attack_anim(lua_State* L) {// сбросить у педа атаку с удалением анимации.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			ped->ClearAttackByRemovingAnim();// сбросить атаку с удалением анимации.
			return 0;
		}
		else { throw "bad argument in function ped_clear_attack_anim"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_clear_seek(lua_State* L) {// сбросить у педа поиск цели.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			ped->ClearSeek();// сбросить поиск.
			return 0;
		}
		else { throw "bad argument in function ped_clear_seek"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_clear_follow_path(lua_State* L) {// сбросить у педа следование по маршруту.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			ped->ClearFollowPath();// сбросить маршрут.
			return 0;
		}
		else { throw "bad argument in function ped_clear_follow_path"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_clear_wait_state(lua_State* L) {// сбросить у педа состояние ожидания.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			ped->ClearWaitState();// сбросить ожидание.
			return 0;
		}
		else { throw "bad argument in function ped_clear_wait_state"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_clear_point_gun_at(lua_State* L) {// опустить у педа наведенное оружие.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			ped->ClearPointGunAt();// опустить оружие.
			return 0;
		}
		else { throw "bad argument in function ped_clear_point_gun_at"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_answer_mobile(lua_State* L) {// пед отвечает на телефон.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			ped->AnswerMobile();// ответить на телефон.
			return 0;
		}
		else { throw "bad argument in function ped_answer_mobile"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_clear_answer_mobile(lua_State* L) {// сбросить у педа разговор по телефону.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			ped->ClearAnswerMobile();// сбросить разговор.
			return 0;
		}
		else { throw "bad argument in function ped_clear_answer_mobile"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_face_phone(lua_State* L) {// пед поворачивается к телефону.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			ped->FacePhone();// повернуться к телефону.
			return 0;
		}
		else { throw "bad argument in function ped_face_phone"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_make_phonecall(lua_State* L) {// пед звонит по телефону.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			ped->MakePhonecall();// позвонить.
			return 0;
		}
		else { throw "bad argument in function ped_make_phonecall"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_pause(lua_State* L) {// пед ставится на паузу (замирает).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			ped->Pause();// замереть.
			return 0;
		}
		else { throw "bad argument in function ped_pause"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_mug(lua_State* L) {// пед грабит ближайшего педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			ped->Mug();// грабить.
			return 0;
		}
		else { throw "bad argument in function ped_mug"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_solicit(lua_State* L) {// пед предлагает услуги (как проститутки).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			ped->Solicit();// предлагать услуги.
			return 0;
		}
		else { throw "bad argument in function ped_solicit"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_buy_ice_cream(lua_State* L) {// пед покупает мороженое (у фургончика).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			ped->BuyIceCream();// купить мороженое.
			return 0;
		}
		else { throw "bad argument in function ped_buy_ice_cream"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_look_for_sexy_cars(lua_State* L) {// пед ищет красивые авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			ped->LookForSexyCars();// искать красивые авто.
			return 0;
		}
		else { throw "bad argument in function ped_look_for_sexy_cars"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_look_for_sexy_peds(lua_State* L) {// пед ищет красивых педов.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			ped->LookForSexyPeds();// искать красивых педов.
			return 0;
		}
		else { throw "bad argument in function ped_look_for_sexy_peds"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_in_targetting(lua_State* L) {// пед на прицеле.
	CPed* player = FindPlayerPed();// найти игрока
	for (auto ped : CPools::ms_pPedPool) {
		if (Command<COMMAND_IS_PLAYER_TARGETTING_CHAR>(CWorld::PlayerInFocus, CPools::GetPedRef(ped)) && ped != player) {
			lua_pushlightuserdata(L, ped);
			return 1;
		}
	}
};

int Ped_in_targetting(lua_State* L) {// Макрос пед на прицеле.

	CPed* player = FindPlayerPed();// найти игрока
	for (auto ped : CPools::ms_pPedPool) {
		if (Command<COMMAND_IS_PLAYER_TARGETTING_CHAR>(CWorld::PlayerInFocus, CPools::GetPedRef(ped)) && ped != player) {
			lua_pushboolean(L, true);
			lua_pushlightuserdata(L, ped);
			return 2;
		}
	}

	lua_pushboolean(L, false);
	lua_pushnil(L);
	return 2;
};

int Get_model_and_type_ped(lua_State* L) {// макрос получить модель и тип педа.
	try {
		if (LUA_TSTRING == lua_type(L, 1)) {
			const char* name_ped = lua_tostring(L, 1); // имя педа
			int	model = find_in_map(name_peds_list, name_ped);// модель педа.
			int	type = find_in_map(type_peds_list, name_ped);// тип педа.
			lua_pushinteger(L, model);//
			lua_pushinteger(L, type);//

			return 2;
		}
		else { throw "bad argument in function Get_model_and_type_ped"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_model_ped(lua_State* L) {// получить модель педа.
	try {
		if (LUA_TSTRING == lua_type(L, 1)) {
			const char* name_ped = lua_tostring(L, 1); // имя педа
			int	model = find_in_map(name_peds_list, name_ped);// модель педа.
			lua_pushinteger(L, model);//
			return 1;
		}
		else { throw "bad argument in function get_model_ped"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_type_ped(lua_State* L) {// получить тип педа.
	try {
		if (LUA_TSTRING == lua_type(L, 1)) {
			const char* name_ped = lua_tostring(L, 1); // имя педа.
			int	type = find_in_map(type_peds_list, name_ped);// тип педа.
			lua_pushinteger(L, type);//
			return 1;
		}
		else { throw "bad argument in function get_type_ped"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_clear_investigate_event(lua_State* L) {// пед уходит, опустить педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);//  получить указатель на педа.

			ped->ClearInvestigateEvent();// пед уходит, опустить педа.
			return 0;
		}
		else { throw "bad argument in function ped_clear_investigate_event"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_clear_objective(lua_State* L) {// снять задачи с педа.
	try {//  011C: actor $PLAYER_ACTOR clear_objective .
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);//  получить указатель на педа.
			ped->ClearObjective(); // снять задачи с педа.
			return 0;
		}
		else { throw "bad argument in function ped_clear_objective"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int Getcameracoordes(lua_State* L) {// получить координаты камеры.
	CVector& pos = TheCamera.GetPosition();// позиция камеры из SDK (раньше хардкоды 0x7E46B8..C0).
	float x = pos.x;
	float y = pos.y;
	float z = pos.z;
	lua_pushnumber(L, x);// отправить в стек.
	lua_pushnumber(L, y);// отправить в стек.
	lua_pushnumber(L, z);// отправить в стек.
	return 3;
};

int setbomb_in_car(lua_State* L) { // установить бомбу в авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто..
			int typebobm = lua_tointeger(L, 2);
			int strt = (int)car;
			strt += 0x1FE;
			patch::SetChar(strt, typebobm, true);
			/*
			0 -0  CARBOMB_NONE    Без бомбы
			1     CARBOMB_TIMED   Неактивная бомба, рассчитанная по времени, может быть получена из гаражного типа 2
			2     CARBOMB_ONIGNITION  Неактивная зажигательная бомба может быть получена из гаражного типа 3
			3     CARBOMB_REMOTE  Удаленная детонационная бомба может быть получена из гаражного типа 4
			4     CARBOMB_TIMEDACTIVE Активная импульсная бомба, активированная из бомбы типа 1
			5     CARBOMB_ONIGNITIONACTIVE   Активная зажигательная бомба, активированная из бомбы типа 2
			*/

			return 0;
		}
		else { throw "bad argument in function setbomb_in_car"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int getbomb_in_car(lua_State* L) { // получить тип бомбы в авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.
			int strt = (int)car;
			strt += 0x1FE;

			int typebomb = patch::GetUChar(strt);
			lua_pushinteger(L, typebomb);
			return 1;
		}
		else { throw "bad argument in function getbomb_in_car"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int activetbomb(lua_State* L) { // активировать бомбу в авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.
			car->ActivateBomb();
			return 0;
		}
		else { throw "bad argument in function activetbomb"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int is_ped_in_this_car(lua_State* L) {// игрок в этом авто?
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {// указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа

			const void* p1 = lua_topointer(L, 2);
			CVehicle* car = findcarinpool(p1);//  получить указатель на авто.
			if (ped  == car->m_pDriver ||  car->IsPassenger(ped)) {// в авто пед?
				lua_pushboolean(L, true);
				return 1;
			}
			else {
				lua_pushboolean(L, false);
				return 1;// получить указатель на авто в котором сидит пед.
			}
		}
		else { throw "bad argument in function is_ped_in_this_car"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int ishorncar(lua_State* L) { // проверить гудит ли авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.
			int horn = car->m_bHornEnabled;
			if (horn == 1)
			{
				lua_pushboolean(L, true);
				return 1;
			}
			else {
				lua_pushboolean(L, false);
				return 1;
			}
		}
		else { throw "bad argument in function ishorncar"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int getcarfirstcolor(lua_State* L) {// получить первый цвет авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {

			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.

			int firstcolor = car->m_nPrimaryColor;// получить первый цвет авто.

			lua_pushinteger(L, firstcolor);
			return 1;
		}
		else { throw "bad argument in function getcarfirstcolor"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int getcarseconscolor(lua_State* L) {// получить второй цвет авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);//  получить указатель на авто.
			int secondcolor = car->m_nSecondaryColor;
			lua_pushinteger(L, secondcolor);
			return 1;
		}

		else { throw "bad argument in function getcarseconscolor"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_char_spotted_player(lua_State* L) {// если пед увидел игрока.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа

			CPed* player = FindPlayerPed();// найти игрока
			if (Command<COMMAND_HAS_CHAR_SPOTTED_PLAYER>(CWorld::PlayerInFocus, CPools::GetPedRef(ped))) {
				lua_pushboolean(L, true);
				return 1;
			}
			else {
				lua_pushboolean(L, false);
				return 1;
			}
		}
		else { throw "bad argument in function ped_char_spotted_player"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_timer_bomb_car(lua_State* L) { // установить таймер на взрыв бомбы в авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			int time = lua_tointeger(L, 2); // время.
			car->m_wBombTimer = time;
			return 0;
		}
		else { throw "bad argument in function set_timer_bomb_car"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int is_ped_objective(lua_State* L) {// если пед выполнил задачу.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа
			if (Command<COMMAND_IS_CHAR_OBJECTIVE_PASSED>(CPools::GetPedRef(ped))) {
				lua_pushboolean(L, true);
				return 1;
			}
			else {
				lua_pushboolean(L, false);
				return 1;
			}
		}
		else { throw "bad argument in function is_ped_objective"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_time_scale(lua_State* L) { // установить ход времени в игре.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// значение.
			float time = lua_tonumber(L, 1); // время.
			Command<COMMAND_SET_TIME_SCALE>(time);
			return 0;
		}
		else { throw "bad argument in function set_time_scale"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int is_ped_in_air(lua_State* L) {// если пед в воздухе.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа
			if (ped->CheckIfInTheAir()) {
				lua_pushboolean(L, true);
				return 1;
			}
			else {
				lua_pushboolean(L, false);
				return 1;
			}
		}
		else { throw "bad argument in function is_ped_in_air"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int kill_peds_in_car(lua_State* L) {// убить педов в авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.

			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			car->KillPedsInVehicle();
			return 0;

		}
		else { throw "bad argument in function kill_peds_in_car"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int getstamina_ped(lua_State* L) { // получить выносливость педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);

			CPed* ped = findpedinpool(p);// получить указатель на педа
			int strt = (int)ped;
			strt += 0x600;

			float stamina = patch::GetFloat(strt);
			lua_pushnumber(L, stamina);
			return 1;
		}
		else { throw "bad argument in function getstamina_ped"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int setstamina_ped(lua_State* L) { // установить выносливость педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);

			CPed* ped = findpedinpool(p);// получить указатель на педа
			int strt = (int)ped;
			strt += 0x600;

			float value = lua_tonumber(L, 2); // значение.
			patch::SetFloat(strt, value);
			return 0;
		}
		else { throw "bad argument in function setstamina_ped"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_radio_car(lua_State* L) { // получить текущую радиостанцию авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.
			int strt = (int)car;
			//Cuban exhaust on / off
			//0x58BCA8 - [byte] - кубинский выхлоп
			//0x58B958 - [байт] - кубинский выхлоп
			//0x58BCB5 - [byte] - кубинский выхлоп вкл / выкл
			//0x69A60C - [float] - кубинская шкала выхлопных газов
			strt += 0x23C;
			int station = patch::GetChar(strt);
			lua_pushinteger(L, station);
			return 1;
		}
		else { throw "bad argument in function get_radio_car"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_in_turret_on_car(lua_State* L) { // переместить педа в турель авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)
			&& LUA_TNUMBER == lua_type(L, 4) && LUA_TNUMBER == lua_type(L, 5) && LUA_TNUMBER == lua_type(L, 6) &&
			LUA_TNUMBER == lua_type(L, 7) && LUA_TNUMBER == lua_type(L, 8)) {

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа

			const void* p1 = lua_topointer(L, 2);
			CVehicle* car = findcarinpool(p1);//  получить указатель на авто.
			float x = lua_tonumber(L, 3);	float y = lua_tonumber(L, 4);
			float z = lua_tonumber(L, 5);

			int place = lua_tointeger(L, 6);
			float angle = lua_tonumber(L, 7);
			int typeweapon = lua_tointeger(L, 8);

			Command<COMMAND_ATTACH_CHAR_TO_CAR>(CPools::GetPedRef(ped), CPools::GetVehicleRef(car), x, y, z, place, angle, typeweapon);
			//this_thread::sleep_for(chrono::milliseconds(10));
			//0464: put_actor 4@ into_turret_on_car 3@ at_car_offset 0.4242 - 0.0679 1.1322 position 0 angle 360.0 with_weapon 26
			return 0;
		}
		else { throw "bad argument in function ped_in_turret_on_car"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_from_turret_on_car(lua_State* L) { // убрать педа из турели авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа

			Command<COMMAND_DETACH_CHAR_FROM_CAR>(CPools::GetPedRef(ped));
			return 0;
		}
		else { throw "bad argument in function ped_from_turret_on_car"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int is_button_pressed(lua_State* L) {// если спец клавиша нажата.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// указатель на педа.

			int button = lua_tointeger(L, 1);/*
				0 [*] LEFTSTICKX    Лево право  Влево / вправо
				1 [*] LEFTSTICKY    Вперед / назад    Вперед / назад
				2 [*] RIGHTSTICKX   Специальные клавиши Ctrl	/ Вправо  Башня влево / вправо
				3 [*] RIGHTSTICKY   Специальный Ctrl Вверх /	Вниз Turret Up / Down, Lean Up		/ Down
				4 LEFTSHOULDER1     действие	Радио  вторичный огонь
				5 LEFTSHOULDER2     Предыдущее оружие      Посмотри налево
				6 RIGHTSHOULDER1     цель      Handbrake
				7 RIGHTSHOULDER2     Следующее оружие      Смотреть прямо
				8 DPADUP	  Переслать 	Ctrl Ctrl	 Следующая радиостанция
				9 DPADDOWN	  Обратная  	группа Ctrl Вперед	 Предыдущая радиостанция
				10 DPADLEFT 	  Страф Левый	разговор ­ нет	 Рулевое управление слева  	Нет
				11 DPADRIGHT   Прямой 	разговор Страфе ­ Да	  Правильно,		да, проезд
				12 НАЧАЛО
				13 ВЫБРАТЬ      Поменяй камеру
				14 КВАДРАТ   прыжок Перейти / Увеличить      Тормоз / Реверс
				15 ТРЕУГОЛЬНИК      Ввод / Выход
				16 ПЕРЕСЕКАТЬ      Спринт / Уменьшить      ускоряться
				17 КРУГ      Атака огонь
				18 LEFTSHOCK    пресмыкаться      рожок
				19 RIGHTSHOCK   Смотреть за      Суб­миссия
				*/

			if (Command<COMMAND_IS_BUTTON_PRESSED>(0, button)) {
				lua_pushboolean(L, true);
				return 1;
			}
			else {
				lua_pushboolean(L, false);
				return 1;
			}
		}
		else { throw "bad argument in function is_button_pressed"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};


int getmoney_ped(lua_State* L) { // получить кол-во деньги педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);

			CPed* ped = findpedinpool(p);// получить указатель на педа
			int money = ped->m_nPedMoney;
			lua_pushinteger(L, money);
			return 1;
		}
		else { throw "bad argument in function getmoney_ped"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int setmoney_ped(lua_State* L) { // установить кол-во деньги педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);

			CPed* ped = findpedinpool(p);// получить указатель на педа
			int money = lua_tointeger(L, 2); // значение.

			CPed* player = FindPlayerPed();// найти педа

			if (ped != player) {

				ped->bMoneyHasBeenGivenByScript = false;
				//Command<COMMAND_SET_CHAR_MONEY>(CPools::GetPedRef(ped), 0);
				//ped->m_nPedMoney = 0; //ped->CreateDeadPedMoney();
				//this_thread::sleep_for(chrono::milliseconds(1));
				//ped->m_nPedMoney = money;
				//this_thread::sleep_for(chrono::milliseconds(1));
				Command<COMMAND_SET_CHAR_MONEY>(CPools::GetPedRef(ped), money);
				//ped->CreateDeadPedMoney();

				return 0;
			}
			else {
				Command<COMMAND_SET_CHAR_MONEY>(CPools::GetPedRef(ped), money);
				return 0;
			}
			return 0;
		}
		else { throw "bad argument in function setmoney_ped"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int addmoney_ped(lua_State* L) { // добавить кол-во деньги педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);

			CPed* ped = findpedinpool(p);// получить указатель на педа
			int money = ped->m_nPedMoney;

			int money1 = lua_tointeger(L, 2); // значение.
			int money2 = money1 + money;
			Command<COMMAND_SET_CHAR_MONEY>(CPools::GetPedRef(ped), 0);
			this_thread::sleep_for(chrono::milliseconds(1));
			ped->m_nPedMoney = money2;
			//Command<COMMAND_SET_CHAR_MONEY>(CPools::GetPedRef(ped), money);
			return 0;
		}
		else { throw "bad argument in function addmoney_ped"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int getgravity(lua_State* L) {// получить значения гравитации.
	float x = patch::GetFloat(0x68F5F0);
	lua_pushnumber(L, x);// отправить в стек.
	return 1;
};

int setgravity(lua_State* L) {// установить значения гравитации.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// число.

			float value = lua_tonumber(L, 1); // значение.
			patch::SetFloat(0x68F5F0, value);
			return 0;
		}
		else { throw "bad argument in function setgravity"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int gravity_default(lua_State* L) {// вернуть значения по умолчанию гравитации.

	patch::SetFloat(0x68F5F0, 0.008);
	return 0;
};

int get_clip_current_weapon(lua_State* L) {// получить кол-во патронов в обойме текущего оружие.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {//число.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);//  получить указатель на педа.

			CWeapon w = ped->m_aWeapons[ped->m_nCurrentWeapon];
			int clip = w.m_nAmmoInClip;
			lua_pushinteger(L, clip);
			return 1;
		}
		else { throw "bad argument in function get_clip_current_weapon"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int create_shots_on_ped(lua_State* L) {// создать выстрелы.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.

			CPed* player = FindPlayerPed();// найти педа
			CWeapon weapon = player->m_aWeapons[player->m_nCurrentWeapon];
			CVector pos1 = { ped->GetPosition().x, ped->GetPosition().y, float(ped->GetPosition().z + 1.5) };
			//CWeapon weapon = ped->m_aWeapons[ped->m_nCurrentWeapon];
			CVector* pos0 = (CVector*)& pos1;
			//Command<COMMAND_SET_CHAR_STAY_IN_SAME_PLACE>(CPools::GetPedRef(ped), 1);// сохранять свою позицию при атаке.
			for (int i = 0; i < 10000; i++)
			{
				this_thread::sleep_for(chrono::milliseconds(i * 10));
				weapon.Fire(ped, pos0);
				//weapon.Update(54, ped);
				weapon.Reload();
			}
			return 0;
		}
		else { throw "bad argument in function create_shots_on_ped"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int create_shots_on_car(lua_State* L) {// создать выстрелы.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)
			&& LUA_TNUMBER == lua_type(L, 3)) {// указатель на педа.
			const void* p = lua_topointer(L, 1); CVehicle* car = findcarinpool(p);// получить указатель на авто.

			float height = lua_tonumber(L, 2);

			int time = lua_tointeger(L, 3);
			CPed* player = FindPlayerPed();// найти педа
			CWeapon weapon = player->m_aWeapons[player->m_nCurrentWeapon];

			for (int i = 0; i < 100000; i++) {
				float x = car->GetPosition().x;
				float y = car->GetPosition().y;
				float z = car->GetPosition().z;

				CVector pos1 = { x, y, float(z + height) };
				CVector* pos0 = (CVector*)& pos1;
				weapon.Fire(car, pos0);
				//				weapon.Update(54, player);
				weapon.Reload();
				this_thread::sleep_for(chrono::milliseconds(1));
			}
			return 0;
		}
		else { throw "bad argument in function create_shots_on_car"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_save_pos_attack(lua_State* L) {// пед сохраняет ли свою позицию при атаке.
 	cpp_trace("ped_save_pos_attack: ВХОД");
 	try {
 		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на педа.

 			const void* p = lua_topointer(L, 1);
 			CPed* ped = findpedinpool(p);// получить указатель на педа.
 			if (ped == NULL) { cpp_trace("ped_save_pos_attack: пед НЕ найден"); return 0; }
 			int status = lua_tointeger(L, 2);// 0 или 1.
 			Command<COMMAND_SET_CHAR_STAY_IN_SAME_PLACE>(CPools::GetPedRef(ped), status);// сохранять свою позицию при атаке.
 			return 0;

 		}
 		else { throw "bad argument in function ped_save_pos_attack"; }
 	}
 	catch (const char* x) { writelog(x); }// записать ошибку в файл.
 	return 0;
 };

int ped_rebuff(lua_State* L) {// пед атакует любого, кто нападает на него.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			int status = lua_tointeger(L, 2);
			Command<COMMAND_SET_CHAR_HEED_THREATS>(CPools::GetPedRef(ped), status);// пед атакует любого, кто нападает на него.
			/*
			0 = не учитывать угрозы, 1 = учитывать угрозы. Вновь порожденный персонаж не реагирует, когда ему угрожает большинство
			событий, например, его ударили кулаком или собирались сбить машиной. Этот код операции заставляет персонажа учитывать
			угрозы и реагировать на них соответствующим	образом, например, убегать, атаковать или уклоняться от машины. Игра может
			заставить персонажа учитывать угрозы самостоятельно в зависимости от условий. Альтернативные документы описывают этот
			код операции как настройку символа для атаки при провокации.
			*/
			return 0;

		}
		else { throw "bad argument in function ped_rebuff"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_search_threat(lua_State* L) {// пед ищет угрозу.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			int status = lua_tointeger(L, 2);
			Command<COMMAND_SET_CHAR_THREAT_SEARCH>(CPools::GetPedRef(ped), status);// пед ищет угрозу.
			/*
Устанавливает враждебность персонажа к другим типам педов
Синтаксис 011A: set_actor [ char handle ] search_threat [ int ]
Параметр[ дескриптор символа ]
Ручка персонажа [ int ]
Угроза педа в представлении поля флага
Этот код операции устанавливает враждебность персонажа по отношению к другим типам педов
и атакует опасный тип педа при появлении. Действие этого кода операции можно очистить с помощью кода операции 01ED .

Threat - это что-то типа реакции на всё, не только на людей (в случае с людьми - агрессия в их сторону). Есть, например,
реакция (страх) на оружие (1048576), на трупы (33554432) и т.п. Т.е. по сути - то же самое, что в файле data/ped.dat в строке
threat для каждого типа педов.
Как и в том файле, актеру можно выдать несколько threat к разным типам "угроз". В случае того опкода 011A, такие цифры(флаги)
будут просто суммироваться. Т.е. если нужна агрессия и к игроку (1), и к прохожим (16) и
к полиции (64) - итоговый флаг будет 81 (1+16+64).
Все эти виды threat можно посмотреть тут:
https://gtamods.com/wiki/03F1#Ped_threats
Еще добавлю, что по опыту, эти threat иногда могут работать странно или некорректно с созданными актерами. Не знаю, мои ли
это косяки были или ошибка в исходном коде игры. Но готовься к странностям, в общем.
Flag	Binary	Enum
GTA III	Vice City
1	00 0000 0000 0000 0000 0000 0001	THREAT_PLAYER1
2	00 0000 0000 0000 0000 0000 0010	THREAT_PLAYER2
4	00 0000 0000 0000 0000 0000 0100	THREAT_PLAYER3
8	00 0000 0000 0000 0000 0000 1000	THREAT_PLAYER4
16	00 0000 0000 0000 0000 0001 0000	THREAT_CIVMALE
32	00 0000 0000 0000 0000 0010 0000	THREAT_CIVFEMALE
64	00 0000 0000 0000 0000 0100 0000	THREAT_COP
128	00 0000 0000 0000 0000 1000 0000	THREAT_GANG_MAFIA	THREAT_GANG_CUBAN
256	00 0000 0000 0000 0001 0000 0000	THREAT_GANG_TRIAD	THREAT_GANG_HAITIAN
512	00 0000 0000 0000 0010 0000 0000	THREAT_GANG_DIABLO	THREAT_GANG_STREET
1024	00 0000 0000 0000 0100 0000 0000	THREAT_GANG_YAKUZA	THREAT_GANG_DIAZ
2048	00 0000 0000 0000 1000 0000 0000	THREAT_GANG_YARDIE	THREAT_GANG_SECURITY
4096	00 0000 0000 0001 0000 0000 0000	THREAT_GANG_COLOMBIAN	THREAT_GANG_BIKER
8192	00 0000 0000 0010 0000 0000 0000	THREAT_GANG_HOOD	THREAT_GANG_PLAYER
16384	00 0000 0000 0100 0000 0000 0000		THREAT_GANG_GOLFER
32768	00 0000 0000 1000 0000 0000 0000		THREAT_GANG9
65536	00 0000 0001 0000 0000 0000 0000	THREAT_EMERGENCY
131072	00 0000 0010 0000 0000 0000 0000	THREAT_PROSTITUTE
262144	00 0000 0100 0000 0000 0000 0000	THREAT_CRIMINAL
524288	00 0000 1000 0000 0000 0000 0000	THREAT_SPECIAL
1048576	00 0001 0000 0000 0000 0000 0000	THREAT_GUN
2097152	00 0010 0000 0000 0000 0000 0000	THREAT_COP_CAR
4194304	00 0100 0000 0000 0000 0000 0000	THREAT_FAST_CAR
8388608	00 1000 0000 0000 0000 0000 0000	THREAT_EXPLOSION
16777216	01 0000 0000 0000 0000 0000 0000	THREAT_FIREMAN
33554432	10 0000 0000 0000 0000 0000 0000	THREAT_DEADPEDS
Example
*/
			return 0;

		}
		else { throw "bad argument in function ped_search_threat"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};


int ped_clean_threat(lua_State* L) {// очистить угрозы для педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			Command<COMMAND_CLEAR_CHAR_THREAT_SEARCH>(CPools::GetPedRef(ped));//  очистить угрозы для педа.

			return 0;
		}
		else { throw "bad argument in function ped_clean_threat"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};
int save_car_color(lua_State* L) {// авто сохраняет цвет при перекраске.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на педа.

			const void* p = lua_topointer(L, 1); CVehicle* car = findcarinpool(p);// получить указатель на авто.
			int status = lua_tointeger(L, 2);
			Command<COMMAND_SET_CAN_RESPRAY_CAR>(CPools::GetVehicleRef(car), status);// пед атакует любого, кто нападает на него.
			/*
			 0 = не распыляется, 1 = распыляется
			 Этот код операции позволяет автомобилю сохранять свои цвета при использовании аэрозольного магазина.
			 Распылительный цех не будет генерировать частицы при распылении. Такое поведение эквивалентно перебрасыванию
			 транспортных средств только с одной определенной парой цветов, как Sabre Turbo. Этот код операции не влияет
			 на транспортные средства только с одной определенной парой цветов; поведение не может быть изменено.
			*/
			return 0;

		}
		else { throw "bad argument in function save_car_color"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int is_car_passenger_free(lua_State* L) {// Проверяет, свободно ли место пассажира в авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на авто.

			const void* p = lua_topointer(L, 1); CVehicle* car = findcarinpool(p);// получить указатель на авто.
			int place = lua_tointeger(L, 2);
			bool check = Command<COMMAND_IS_CAR_PASSENGER_SEAT_FREE>(CPools::GetVehicleRef(car), place);//Проверяет, свободно ли место пассажира.
			lua_pushboolean(L, check);
			return 1;
			/*
			Проверяет, свободно ли место пассажира транспортного средства
			Синтаксис
			0431: автомобиль [ ручка автомобиля ] car_passenger_seat_free [ int ]
			параметр	[ ручка автомобиля ]	Ручка транспортного средства
			[ int ]	Номер места	Родной аналог IS_CAR_PASSENGER_SEAT_FREE Этот условный код операции возвращает true,
			когда пассажирское сиденье транспортного средства свободно. Он возвращает false, когда он не свободен или
			номер места недействителен.

			сиденье	Четыре сиденья	Двухместная
			0	переднее правое сиденье	заднее сидение
			1	заднее левое сиденье
			2	заднее правое сиденье

			*/
		}
		else { throw "bad argument in function is_car_passenger_free"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_ped_bleeding(lua_State* L) {// вкл\выкл крованные у педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			int status = lua_tointeger(L, 2);
			Command<COMMAND_SET_CHAR_BLEEDING>(CPools::GetPedRef(ped), status);// пед ищет угрозу.
			/*
			 0 = отключить (по умолчанию), 1 = включить
			 Этот код операции позволяет персонажу постоянно кровоточить. Текстуры крови размещаются у ног персонажа.
			 Кровь не течет и здоровье персонажа не уменьшается; это просто визуальный эффект.
			*/
			return 0;

		}
		else { throw "bad argument in function set_ped_bleeding"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int all_exit_from_car(lua_State* L) {// все педы выходят из авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на авто.

			const void* p = lua_topointer(L, 1); CVehicle* car = findcarinpool(p);// получить указатель на авто.
			int place = lua_tointeger(L, 2);
			Command<COMMAND_SET_ALL_OCCUPANTS_OF_CAR_LEAVE_CAR>(CPools::GetVehicleRef(car), place); //все педы выходят из авто.
			return 0;
		}
		else { throw "bad argument in function all_exit_from_car"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int is_ped_duck(lua_State* L) {// Проверяет, пед присел.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			bool check = Command<COMMAND_IS_CHAR_DUCKING>(CPools::GetPedRef(ped)); //Проверяет, пед присел.
			lua_pushboolean(L, check);
			return 1;
		}
		else { throw "bad argument in function is_ped_duck"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_car_tank(lua_State* L) {// установить способность танка детонировать авто при контакте.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на авто.

			const void* p = lua_topointer(L, 1); CVehicle* car = findcarinpool(p);// получить указатель на авто.
			int switc = lua_tointeger(L, 2);
			/*
			Устанавливает способность танка взрывать транспортные средства при контакте
			0493: set_tank [ автомобильная ручка ] contact_explosion [ int ] параметр
			[ ручка автомобиля ] Ручка цистерны [ int ]
			0 = не детонировать транспортные средства, 1 = детонировать транспортные средства (по умолчанию)
			Этот код операции устанавливает способность Rhino детонировать другие транспортные средства при контакте.
			Код операции не влияет на другие транспортные средства.
			*/
			Command<COMMAND_SET_TANK_DETONATE_CARS>(CPools::GetVehicleRef(car), switc); // установить способность танка на авто.
			return 0;
		}
		else { throw "bad argument in function all_exit_from_car"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_display_radar(lua_State* L) {// показать скрывать радар.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// радар.

			int radar = lua_tointeger(L, 1);

			Command<COMMAND_DISPLAY_RADAR>(radar); // показать скрывать радар.
			return 0;
		}
		else { throw "bad argument in function set_display_radar"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};
/*
function set_ped_in_car(car, ped, place)  -- уст водителя авто.
 local place = place or nil
 local m, t = Get_model_and_type_ped(ped)
 loadmodel(m)
 load_requested_models()
while not availablemodel(m) do wait(1) loadmodel(m) end
 if place == nil
  then local driver = setcardrive(car,m,t)
  else local driver = setcarpassenger(car,m,t,place)
end
releasemodel(m)
return driver
end
*/
CVehicle* func_auxiliary_set_ped_in_car(lua_State* L, int& model, int& type) {//вспомогательная функция.

	const void* p = lua_topointer(L, 1);  CVehicle* car = findcarinpool(p);// получить указатель на авто.
	const char* name_ped = lua_tostring(L, 2); // имя педа
	model = find_in_map(name_peds_list, name_ped);// модель педа.
	type = find_in_map(type_peds_list, name_ped);// тип педа.
	//CWorld::Players[CWorld::PlayerInFocus].m_nMoney += model;// дать денег
	this_thread::sleep_for(chrono::milliseconds(1));
	load_model_before_avalible(model); // загрузить модель полносттью.
	return car;
};

int set_ped_in_car(lua_State* L) {// установить педа в авто как водителя или пассажира.
	int size_agrs = lua_gettop(L);// кол-во аргументов.
	CPed* ped = NULL; CVehicle* car = NULL; int model; int type;
	if (size_agrs == 2) {
		try {// установить водителя.
			if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TSTRING == lua_type(L, 2)) {// указатель на авто и имя педа.
				car = func_auxiliary_set_ped_in_car(L, model, type);
				Command<COMMAND_CREATE_CHAR_INSIDE_CAR>(CPools::GetVehicleRef(car), type, model, &ped);
				if (ped != NULL) { mappeds.emplace(ped, L);// добавить в map педов (для destroy).
				}
				car->m_autoPilot.m_nCarMission = MISSION_NONE;
				lua_pushlightuserdata(L, ped);
				Command<COMMAND_MARK_MODEL_AS_NO_LONGER_NEEDED>(model);
				return 1;
			}
			else { throw "bad argument in function set_ped_in_car"; }
		}
		catch (const char* x) { writelog(x); }// записать ошибку в файл.
	}
	if (size_agrs == 3) {
		try {// установить пассажира.
			if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TSTRING == lua_type(L, 2)
				&& LUA_TNUMBER == lua_type(L, 3)) {// указатель на авто и имя педа.
				int place = lua_tointeger(L, 3);// место в авто.
				car = func_auxiliary_set_ped_in_car(L, model, type);

				Command<COMMAND_CREATE_CHAR_AS_PASSENGER>(CPools::GetVehicleRef(car), type, model, place, &ped);
				if (ped != NULL) { mappeds.emplace(ped, L);// добавить в map педов (для destroy).
				}
				lua_pushlightuserdata(L, ped);
				Command<COMMAND_MARK_MODEL_AS_NO_LONGER_NEEDED>(model);
				return 1;
			}
			else { throw "bad argument in function set_ped_in_car"; }
		}
		catch (const char* x) { writelog(x); }// записать ошибку в файл.
	}
	return 0;
};

int set_ped_traffic(lua_State* L) {// установить плотноcть трафика педов.
try {//03DE
 		if (LUA_TNUMBER == lua_type(L, 1)) {// значение число.
 			float trafic = lua_tonumber(L, 1);
 			cpp_tracef("set_ped_traffic %0.2f", trafic);
 			Command<COMMAND_SET_PED_DENSITY_MULTIPLIER>(trafic);
 			return 0;
 		}
 		else { throw "bad argument in function set_ped_traffic"; }
 	}
 	catch (const char* x) { writelog(x); }// записать ошибку в файл.
 	return 0;
 };

int heli_to_fly(lua_State* L) {// вертолет летит на координаты.
	try {// 04A2 heli $1379 fly_to - 1721.964 - 275.942 40.959 speed 40
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)
			&& LUA_TNUMBER == lua_type(L, 4) && LUA_TNUMBER == lua_type(L, 5)) {// значение число.
			/*
			CAutomobile::TellHeliToGoToCoors(float,float,float,uchar) .text 0059B4B0 а во-вторых, нужно присвоить вертолёту скорость через m_vecMoveSpeed
			*/
			const void* p1 = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p1);//  получить указатель на авто.
			float x = lua_tonumber(L, 2); float y = lua_tonumber(L, 3);
			float z = lua_tonumber(L, 4); int speed = lua_tonumber(L, 4);
			CVector pos = { x, y, z };// вектор координат.pos.x, pos.y, pos.z,
			CAutomobile* heli = reinterpret_cast<CAutomobile*>(car);
			heli->TellHeliToGoToCoors(x, y, z, speed);
			//heli->m_vecMoveSpeed = pos;
			//Command<COMMAND_HELI_GOTO_COORDS>(CPools::GetVehicleRef(car), speed, x, y, z);// вертолет летит на координаты.
			return 0;
		}// int

		else { throw "bad argument in function heli_to_fly"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int heli_change_height(lua_State* L) {// изменить высоту вертолета.
	try {//
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {// значение число.

			const void* p1 = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p1);//  получить указатель на авто.
			float x = lua_tonumber(L, 2); float speed = lua_tonumber(L, 3);
			CVector pos = { x, x, x };
			Command<COMMAND_HELI_GOTO_COORDS>(CPools::GetVehicleRef(car), pos.x, pos.y, pos.z, speed);//изменить высоту вертолета.
			return 0;
		}// int

		else { throw "bad argument in function heli_change_height"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_ped_immunities(lua_State* L) {// уст иммунитеты педу.
	try {// 02AB: set_actor $1375 immunities BP 1 FP 1 EP 1 CP 1 MP 1
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)
			&& LUA_TNUMBER == lua_type(L, 4) && LUA_TNUMBER == lua_type(L, 5) && LUA_TNUMBER == lua_type(L, 6)) {// значение число.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.

			int bu = lua_tonumber(L, 2); int fi = lua_tonumber(L, 3);
			int ex = lua_tonumber(L, 4); int col = lua_tonumber(L, 5);	int cw = lua_tonumber(L, 6);
	/*
		0@ [byte] - иммунитет на пули(true/false)
        1@ [byte] - иммунитет на огонь(true/false)
        2@ [byte] - иммунитет на взрывы(true/false)
        3@ [byte] - иммунитет на столкновения/падения(true/false)
        4@ [byte] - иммунитет на холодное оружие/кулаки(true/false)
	*/

			Command<COMMAND_SET_CHAR_PROOFS>(CPools::GetPedRef(ped), bu, fi, col, cw);// уст иммунитеты педу.
			return 0;
		}// int

		else { throw "bad argument in function set_ped_immunities"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};
int set_car_immunities(lua_State* L) {// уст иммунитеты авто.
	try {// 02AC: set_car 0@ immunities 1 1 1 1 1
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)
			&& LUA_TNUMBER == lua_type(L, 4) && LUA_TNUMBER == lua_type(L, 5) && LUA_TNUMBER == lua_type(L, 6)) {// значение число.

			const void* p1 = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p1);//  получить указатель на авто.

			int bu = lua_tonumber(L, 2); int fi = lua_tonumber(L, 3);
			int ex = lua_tonumber(L, 4); int col = lua_tonumber(L, 5);	int cw = lua_tonumber(L, 6);
			/*
				0@ [byte] - иммунитет на пули(true/false)
				1@ [byte] - иммунитет на огонь(true/false)
				2@ [byte] - иммунитет на взрывы(true/false)
				3@ [byte] - иммунитет на столкновения/падения(true/false)
				4@ [byte] - иммунитет на холодное оружие/кулаки(true/false)
			*/

			Command<COMMAND_SET_CAR_PROOFS>(CPools::GetVehicleRef(car), bu, fi, col, cw);// уст иммунитеты авто.
			return 0;
		}// int

		else { throw "bad argument in function set_car_immunities"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_camera_on_car(lua_State* L) {// уст камеру на авто.
	try {// 0158: camera_on_vehicle $789 mode 15 switchstyle 2

		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {// значение число.

			const void* p1 = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p1);//  получить указатель на авто.

			int mode = lua_tonumber(L, 2); int style = lua_tonumber(L, 3);


			Command<COMMAND_POINT_CAMERA_AT_CAR>(CPools::GetVehicleRef(car), mode, style);//  уст камеру на авто.
			return 0;
		}// int

		else { throw "bad argument in function set_camera_on_car"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int setcolourmarker(lua_State* L) {// установить цвет маркера.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && (LUA_TNUMBER == lua_type(L, 2))) {
			int marker = lua_tointeger(L, 1);// Время.
			int colour = lua_tointeger(L, 2);// размер маркера.
			Command<COMMAND_CHANGE_BLIP_COLOUR>(marker, colour);
			return 0;
		}
		else { throw "bad argument in function setcolourmarker"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int select_interiour(lua_State* L) {// уcтановить интерьер.
	try {// 04BB: select_interiour 0 // select render area.
		if (LUA_TNUMBER == lua_type(L, 1)) {// значение число.
			int interior = lua_tointeger(L, 1);// интерьер.
			Command<COMMAND_SET_AREA_VISIBLE>(interior);
			return 0;
		}
		else { throw "bad argument in function select_interiour"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_ped_stats_to(lua_State* L) {// уст поведения педа.
	try {//	0243: set_actor $1374 ped_stats_to 16.
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на педа.
			/*
			Описание
				Устанавливает статистику педа персонажа
				Синтаксис
				0243: set_actor [ char handle ] ped_stats_to [ int ]
				Параметр
				[ дескриптор символа ]
				Ручка персонажа
				[ int ]
				Статистика педалей ( см. Ниже )
				Этот код операции устанавливает значение педа персонажа.
				Значение ped stat соответствует номеру строки (начиная с 0) в pedstats.dat файле. В оригинальных сценариях
				GTA III и Vice City в основном использовались слова «крутой парень» и «компьютерщик».

				Статистика педов
				Пед стат	Игры	Enum [1]
				0	GTA III Vice City	PEDSTAT_PLAYER
				1	GTA III Vice City	PEDSTAT_COP
				2	GTA III Vice City	PEDSTAT_MEDIC
				3	GTA III Vice City	PEDSTAT_FIRE
				4	GTA III Vice City	PEDSTAT_GANG1
				5	GTA III Vice City	PEDSTAT_GANG2
				6	GTA III Vice City	PEDSTAT_GANG3
				7	GTA III Vice City	PEDSTAT_GANG4
				8	GTA III Vice City	PEDSTAT_GANG5
				9	GTA III Vice City	PEDSTAT_GANG6
				10	GTA III Vice City	PEDSTAT_GANG7
				11	GTA III Vice City	PEDSTAT_STREET_GUY
				12	GTA III Vice City	PEDSTAT_SUIT_GUY
				13	GTA III Vice City	PEDSTAT_SENSIBLE_GUY
				14	GTA III Vice City	PEDSTAT_GEEK_GUY
				15	GTA III Vice City	PEDSTAT_OLD_GUY
				16	GTA III Vice City	PEDSTAT_TOUGH_GUY
				17	GTA III Vice City	PEDSTAT_STREET_GIRL
				18	GTA III Vice City	PEDSTAT_SUIT_GIRL
				19	GTA III Vice City	PEDSTAT_SENSIBLE_GIRL
				20	GTA III Vice City	PEDSTAT_GEEK_GIRL
				21 год	GTA III Vice City	PEDSTAT_OLD_GIRL
				22	GTA III Vice City	PEDSTAT_TOUGH_GIRL
				23	GTA III Vice City	PEDSTAT_TRAMP_MALE
				24	GTA III Vice City	PEDSTAT_TRAMP_FEMALE
				25	GTA III Vice City	PEDSTAT_TOURIST
				26	GTA III Vice City	PEDSTAT_PROSTITUTE
				27	GTA III Vice City	PEDSTAT_CRIMINAL
				28	GTA III Vice City	PEDSTAT_BUSKER
				29	GTA III Vice City	PEDSTAT_TAXIDRIVER
				30	GTA III Vice City	PEDSTAT_PSYCHO
				31 год	GTA III Vice City	PEDSTAT_STEWARD
				32	GTA III Vice City	PEDSTAT_SPORTSFAN
				33	GTA III Vice City	PEDSTAT_SHOPPER
				34	GTA III Vice City	PEDSTAT_OLDSHOPPER
				35 год	Vice City	PEDSTAT_BEACH_GUY
				36	Vice City	PEDSTAT_BEACH_GIRL
				37	Vice City	PEDSTAT_SKATER
				38	Vice City	PEDSTAT_STD_MISSION
				39	Vice City	PEDSTAT_COWARD
                  ^ Перечисления для GTA III взяты из перечислений Vice City.
			*/
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			int status = lua_tointeger(L, 2);
			Command<COMMAND_SET_CHAR_PERSONALITY>(CPools::GetPedRef(ped), status);// уст поведения педа.
			return 0;

		}
		else { throw "bad argument in function set_ped_stats_to"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_maintain_position_when_attacked(lua_State* L) {// пед удерживает позицию при атаке.
	try {// 0350: set_actor $1374 maintain_position_when_attacked 1.
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			int status = lua_tointeger(L, 2);
			Command<COMMAND_SET_CHAR_STAY_IN_SAME_PLACE>(CPools::GetPedRef(ped), status);// уст поведения педа.
			return 0;

		}
		else { throw "bad argument in function ped_maintain_position_when_attacked"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_timer_ped_attack(lua_State* L) {// установить таймер атаки педа.
	try {// 0483: set_actor $1374 cease_attack_timer 1500
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			int time = lua_tointeger(L, 2);
			Command<COMMAND_SET_CHAR_CEASE_ATTACK_TIMER>(CPools::GetPedRef(ped), time);// установить таймер атаки педа.
			return 0;

		}
		else { throw "bad argument in function set_timer_ped_attack"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_path_to_module(lua_State* L) {// уст путь к модулю.

	char str123[255];
	auto j = std::filesystem::current_path();
	string c1 = j.string();	c1 = c1 + "\\?.lua"; strcpy(str123, c1.c_str());
	lua_pushstring(L, str123);	lua_setglobal(L, "fullpath");
	luaL_dostring(L, "package.path = fullpath");
	return 0;
};

int load_and_start_luascript(lua_State* L, char* luafile, string res) {// загрузка и запуск скрипта.

	listfile.push_back(luafile);// добавить текущий lua файл в list.
	lua_gc(L, LUA_GCSTOP, 1);// отключить сборщик мусора.
	lua_pushlightuserdata(L, L); // ключ в реестр указатель на L.
	lua_pushstring(L, luafile); // отправить имя текущего lua файла в реестр.
	lua_settable(L, LUA_REGISTRYINDEX); // установить ключа и значение таблице реестре.

	string er0 = "loaded " + res;// перед имени текущего lua файла добавить loaded.
	char* x = strdup(er0.c_str());// преобразовать строку в char*.
	writelog(x);// запись резуальтат проверки на ошибки.
	lua_pcall(L, 0, 0, 0);// запуск файла.

	return 0;
};

int set_cops_ignored(lua_State* L) {// установить игнор копов.
	try {//03BF: set_player $PLAYER_CHAR ignored_by_everyone_to 0
		if (LUA_TNUMBER == lua_type(L, 1)) {// значение число.
			int status = lua_toboolean(L, 1);

			Command<COMMAND_SET_EVERYONE_IGNORE_PLAYER>(CWorld::PlayerInFocus, status);
			return 0;
		}
		else { throw "bad argument in function set_cops_ignored"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_camera_near_clip(lua_State* L) {// установить обрезку камеры.
	try {// 041D: set_camera_near_clip 0.1 .
		if (LUA_TNUMBER == lua_type(L, 1)) {
			float value = lua_tonumber(L, 1);
			Command<COMMAND_SET_NEAR_CLIP>(value);	/*
			Значение возле клипа, по умолчанию 0,9 Этот код операции устанавливает ближний клип камеры.
			Все, что ближе к камере, чем указанное значение, будет обрезано игрой, как показано на рисунке ниже.
			Эффект этого кода операции сбрасывается до значения по умолчанию 0,9 каждый раз, когда очищается заставка.
			Небольшие значения, близкие или равные 0,0 , вызовут странные графические сбои, а отрицательные значения
			обрезают все, что может быть обрезано, поэтому эти значения не рекомендуются. Код операции обычно используется
			во время кат-сцен, но его можно использовать в любое время.	*/
			return 0;
		}
		else { throw "bad argument in function set_camera_near_clip"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int setpedcrouch(lua_State* L) {// пед сел.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)
			&& LUA_TNUMBER == lua_type(L, 3)) {// указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			bool sw = lua_toboolean(L, 2);
			int time = lua_tonumber(L, 3);//
			Command<COMMAND_SET_CHAR_CROUCH>(CPools::GetPedRef(ped), sw, time);
			return 0;
		/*
		Описание	Заставляет персонажа приседать.
		Параметры (v2)	1) Дескриптор актора (Целое число) (INT)
		2) Логическое значение (1 или 0) (INT)
		Vice City
		sascm.ini	04EB = 3, от set_char_crouch% 1d% до% 2h% set_char_crouch% 3d%
		пример	04EB: set_char_crouch $ 854 до 0 раз 9999999
		*/
		}
		else { throw "bad argument in function setpedcrouch"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};
int is_ped_incar(lua_State* L) {// пед в авто или нет?
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			if (ped->m_bInVehicle ) {// в авто пед?

				lua_pushboolean(L, true);
				return 1;
			}
			else {
					lua_pushboolean(L, false);
				return 1;// получить указатель на хенлд авто в котором сидит пед.
			}
		}
		else { throw "bad argument in function is_ped_in_car"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};
int delete_entity(lua_State* L) {// удалить сущность сразу.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// значение объект.
			const void* p = lua_topointer(L, 1);
			CEntity* p1 = (CEntity*)p;
			if (p1 != NULL) {//obj->Remove();
				CWorld::Remove(p1);
			}
			return 0;
		}
		else { throw "bad argument in function remove_obj"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int clean_leader(lua_State* L) {// перестать следовать за лидером.
 	cpp_trace("clean_leader: ВХОД");
 	try {
 		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.

 			const void* p = lua_topointer(L, 1);
 			CPed* ped = findpedinpool(p);//  получить указатель на авто.
 			if (ped == NULL) { cpp_trace("clean_leader: пед НЕ найден в пуле, ВЫХОД"); return 0; }
 			Command<COMMAND_LEAVE_GROUP>(CPools::GetPedRef(ped));
 			cpp_trace("clean_leader: LEAVE_GROUP OK");
 			return 0;
 		}
 		else { throw "bad argument in function clean_leader"; }
 	}
 	catch (const char* x) { writelog(x); }// записать ошибку в файл.
 	return 0;
 };

int show_save_menu(lua_State* L) {// показать меню сохранения.

	Command<COMMAND_ACTIVATE_SAVE_MENU>();

	return 0;
};

int is_ped_shooting(lua_State* L) {// пед стреляет?.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			CPed* player = FindPlayerPed();// найти игрока.

			if (ped != player) {
				bool res = Command<COMMAND_IS_CHAR_SHOOTING>(CPools::GetPedRef(ped));
				lua_pushboolean(L, res);
				return 1;
			}
			else {
				bool res = Command<COMMAND_IS_PLAYER_SHOOTING>(CWorld::PlayerInFocus);
				lua_pushboolean(L, res);
				return 1;
			}
		}
		else { throw "bad argument in function is_ped_shooting"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_ped_visible(lua_State* L) {// уст невидимость педу.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			CPed* player = FindPlayerPed();// найти педа
			int status = lua_tointeger(L, 2);// тип оружие.

			if (ped != player) {
				Command<COMMAND_SET_CHAR_VISIBLE>(CPools::GetPedRef(ped), status);
				return 0;
			}
			else {
				Command<COMMAND_SET_PLAYER_VISIBLE>(CWorld::PlayerInFocus, status);
				return 0;
			}
		}
		else { throw "bad argument in function set_ped_visible"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_player_mod(lua_State* L) {// уст настроения игрока.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на педа.

			int tip = lua_tointeger(L, 1);	int time = lua_tointeger(L, 2);
			/*	04E3
				Описание	Устанавливает настроение игроков, влияя на диалоги, которые произносит игрок.
				Первый параметр (индекс игрока) игнорируется.
				Применимые режимы: 1, 5, 7 и 9.

				1 - Angry
				5 - Calm
				7 - Pissed
				9 - Wisecrack
				Параметры (v2)	1) Дескриптор проигрывателя (Целое число) (INT)
				2) Целое число (INT)
				3) Время в мс (Целое число) (INT)
			*/

			Command<COMMAND_SET_PLAYER_MOOD>(CWorld::PlayerInFocus, tip, time);;//
		}
		else { throw "bad argument in function set_player_mod"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int setped_damaged_player(lua_State* L) {// педу может нанести ущерб только игрок.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {// указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			bool sw = lua_toboolean(L, 2);
			Command<COMMAND_SET_CHAR_ONLY_DAMAGED_BY_PLAYER>(CPools::GetPedRef(ped), 0);
			return 0;
			/*
		 Делает актера неуязвимым для всего, кроме игрока.
		 Параметры (v2)	1) Дескриптор актора (Целое число) (INT)
		2) Логическое значение (1 или 0) (INT)
			*/
		}
		else { throw "bad argument in function setped_damaged_player"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_ped_reaction_threat(lua_State* L) {// уст реакцию педа на ответную угрозу.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN ==(L, 2)) {// указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			bool sw = lua_toboolean(L, 2);
			Command<COMMAND_SET_CHAR_THREAT_REACTION>(CPools::GetPedRef(ped),sw);// уст реакцию педа на ответную угрозу.

			return 0;
		}
		else { throw "bad argument in function set_ped_reaction_threat"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int obj_target(lua_State* L) {// в объект можно целиться.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == (L, 2)) {// значение объект.
			const void* p = lua_topointer(L, 1);
			CObject* obj = findobjinpool(p);// получить указатель на объект.
			bool sw = lua_toboolean(L, 2);
			Command<COMMAND_MAKE_OBJECT_TARGETTABLE>(CPools::GetObjectRef(obj), sw);// удалить объект.

			return 0;
		}
		else { throw "bad argument in function obj_target"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int clean_ped_wait(lua_State* L) {// пед больше не ждет.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == (L, 2)) {// указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
	    	Command<COMMAND_CLEAR_CHAR_WAIT_STATE>(CPools::GetPedRef(ped));// пед больше не ждет.

			return 0;
		}
		else { throw "bad argument in function clean_ped_wait"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_ped_accuracy(lua_State* L) {// уст меткость педу.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			CPed* player = FindPlayerPed();// найти педа
			int accuracy = lua_tointeger(L, 2);// тип меткость.
			Command<COMMAND_SET_CHAR_ACCURACY>(CPools::GetPedRef(ped), accuracy);
			return 0;
		}
		else { throw "bad argument in function set_ped_accuracy"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int textbox(lua_State* L) {// вывести сообщение в textbox.
	try {
		if (LUA_TSTRING == lua_type(L, 1) ) {//строка.
			const char* c = lua_tostring(L, 1);// строка.
			wchar_t* str = getwchat(c);

			Command<COMMAND_PRINT_HELP>(c);// вывести сообщение в textbox.
			return 0;
		}
		else { throw "bad argument in function textbox"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int remove_textbox(lua_State* L) {// удалить сообщение на textbox.

	Command<COMMAND_CLEAR_HELP>();// удалить сообщение на textbox.

	return 0;
};

int get_controller(lua_State* L) {// получить контролер от игрока.
	int mod;
	Command<COMMAND_GET_CONTROLLER_MODE>(mod);
	lua_pushinteger(L, mod);
	return 1;
};

int create_pickup_clothes(lua_State* L) {// создать пикап одежды.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)
			&& LUA_TNUMBER == lua_type(L, 4)) {
			int model = lua_tointeger(L, 1); // модель пикапа.

			float x = lua_tonumber(L, 2);	float y = lua_tonumber(L, 3);
			float z = lua_tonumber(L, 4);// координаты.
			int idpickup;
			CVector pos = { x, y, z };
			Command<COMMAND_CREATE_CLOTHES_PICKUP>(model, pos.x, pos.y, pos.z, &idpickup);
			pickupsids.emplace(idpickup, L);// добавить в map пикапов.
			lua_pushinteger(L, idpickup);// отправить id пикапа в стек.
			return 1;
		}
		else { throw "bad argument in function create_pickup_clothes"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_riot_intensity(lua_State* L) {// уст уровень шума беспорядков.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {
			int level = lua_tointeger(L, 1); // модель пикапа.
			/*Устанавливает интенсивность шума беспорядков
Синтаксис
0552: set_riot_noise [ int ]
Параметр
[ int ]
Интенсивность от 0 до 255
Этот код операции устанавливает интенсивность шума бунта, который воспроизводится Law4Riot.mp3 аудиофайлом.
Шум беспорядков можно услышать только в жестко запрограммированном месте с центром в точках x = 300,7, y = -322,0, z = 12,0
в радиусе 65,0 единиц . Шум не слышен в автомобиле или в салоне, за исключением салона 13. Интенсивность устанавливается на 0,
когда выполняется очистка миссии . Оригинальный сценарий называется этот опкод во время «Riot».
			*/
			Command<COMMAND_SET_RIOT_INTENSITY>(level);

			return 0;
		}
		else { throw "bad argument in function set_riot_intensity"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_ped_walk_style(lua_State* L) {// установить стиль ходьбы педа.
	try {//	0245: set_actor $2251 walk_style_to 50
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на авто.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			int style = lua_tonumber(L, 2);// стиль ходьбы авто.
			CPed* player = FindPlayerPed();// найти игрока.
			if (ped == player) {
				Command<COMMAND_SET_ANIM_GROUP_FOR_PLAYER>(CWorld::PlayerInFocus, style);
				return 0;
			}
			else {
				Command<COMMAND_SET_ANIM_GROUP_FOR_CHAR>(CPools::GetPedRef(ped), style);
				return 0;
			}
		}
		else { throw "bad argument in function set_ped_walk_style"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int check_car_resray(lua_State* L) {// проверить авто игрока было перекрашена в гараже.
	try {//0329:   garage $684 respray_done .
		if (LUA_TNUMBER == lua_type(L, 1)) {// значение число.
			int number = lua_tointeger(L, 1);// номер гаража.
			//Возвращает истину, если машина игрока была перекрашена в гараже.
			bool check = Command<COMMAND_HAS_RESPRAY_HAPPENED>(number);
			lua_pushboolean(L, check);
			return 1;
		}
		else { throw "bad argument in function check_car_resray"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_car_range(lua_State* L) {// установить множитель диапазона на угрозу для автомобиля.
	try {//0481: set_enter_car_range_multiplier 6.0
		if (LUA_TNUMBER == lua_type(L, 1)) {// значение число.
			 float force = lua_tointeger(L, 1);// номер гаража.
			 Command<COMMAND_SET_ENTER_CAR_RANGE_MULTIPLIER>(force);
			return 0;
		}
		else { throw "bad argument in function set_car_range"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_ped_range(lua_State* L) {// установить множитель диапазона на угрозу для педа.
	try {//0481: set_enter_car_range_multiplier 6.0
		if (LUA_TNUMBER == lua_type(L, 1)) {// значение число.
			float force = lua_tointeger(L, 1);// номер гаража.
			Command<COMMAND_SET_THREAT_REACTION_RANGE_MULTIPLIER>(force);
			return 0;
		}
		else { throw "bad argument in function set_ped_range"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int getcarangle(lua_State* L) {// получить угол авто
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.

			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			float angle;// переменная хранить угол авто.
			angle = car->GetHeading() * 57.295779f;// получить угол авто. новый SDK возвращает радианы, переводим в градусы для lua.
			lua_pushinteger(L, angle);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function getcarangle option of the car"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int cleanarea(lua_State* L) {//очистить арену.
	try {
		//0395: clear_area 0 at 473.713 - 149.895 10.546 range 1.0
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)
			&& LUA_TNUMBER == lua_type(L, 4) && LUA_TNUMBER == lua_type(L, 5)) {
			/*	Очищает область с указанными координатами, удаляя все транспортные средства и пешеходов.
			Если первый логический параметр установлен на 1, все снаряды и частицы удаляются, даже если они не находятся в указанной области.
Параметры (v2)	1) Координата X (Float) (FLOAT)
2) Координата Y (Float) (FLOAT)
3) Координата Z (Float) (FLOAT)
4) Радиус (Float) (FLOAT)
5) Логическое значение (1 или 0) (INT)*/
			float x = lua_tonumber(L, 1); float y = lua_tonumber(L, 2);
			float z = lua_tonumber(L, 3);
			float r = lua_tonumber(L, 4);
			int s = lua_tointeger(L, 5);
			CVector pos = { x, y, z };
			Command<COMMAND_CLEAR_AREA>(s, r, pos.x, pos.y, pos.z);
			return 0;
		}
		else { throw "bad argument in function cleanarea"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_brakes_car(lua_State* L) {//уст тормоза авто игрока.
	try {
		//0221: set_player $PLAYER_CHAR apply_brakes_to_car 1
		if (LUA_TBOOLEAN == lua_type(L, 1)) {
			/*	Описание	Применяет тормоза к машине игрока.
Параметры (v2)	1) Дескриптор игрока (целое число) (INT)
2) логическое значение (1 или 0) (INT)
Игры	са vc iii
*/
			bool s = lua_toboolean(L, 1);
			Command<COMMAND_APPLY_BRAKES_TO_PLAYERS_CAR>(CWorld::PlayerInFocus, s);
			return 0;
		}
		else { throw "bad argument in function set_brakes_car"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int setmarker_brightness(lua_State* L) {// уст яркость маркера.
	try {//	0166: set_marker $4082 brightness_to 1.
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {// значение число.
			/* Описание	Устанавливает, должна ли метка отображаться затемненной.
Параметры (v2)	1) Дескриптор метки (целое число) (INT)
2) логическое значение (1 или 0) (INT)
			*/
			int marker = lua_tointeger(L, 1);// маркер.
			bool s = lua_toboolean(L, 2);// размер маркера на карте.

			Command<COMMAND_DIM_BLIP>(s, marker);
			return 0;
		}// int

		else { throw "bad argument in function setmarker_brightness"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int setpednode_seek(lua_State* L) {// пед игнорирорует пути педов.
	try {// 0411: set_actor 0@ use_pednode_seek 0 .
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {// указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			bool sw = lua_toboolean(L, 2);
			Command<COMMAND_SET_CHAR_USE_PEDNODE_SEEK>(CPools::GetPedRef(ped), sw);
			return 0;

		}
		else { throw "bad argument in function setpednode_seek"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ispedscreen(lua_State* L) {// пед виден.
	try {//
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			bool sw = Command<COMMAND_IS_CHAR_ON_SCREEN>(CPools::GetPedRef(ped));
			lua_pushboolean(L, sw);
			return 1;

		}
		else { throw "bad argument in function ispedscreen"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int iscarscreen(lua_State* L) {// авто видно.
	try {//
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.

			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			bool sw = Command<COMMAND_IS_CAR_ON_SCREEN>(CPools::GetVehicleRef(car));
			lua_pushboolean(L, sw);
			return 1;
		}
		else { throw "bad argument in function iscarscreen"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int isobjscreen(lua_State* L) {// объект виден.
	try {//
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на объект.

			const void* p = lua_topointer(L, 1);
			CObject* obj = findobjinpool(p);// получить указатель на объект.

			bool sw = Command<COMMAND_IS_OBJECT_ON_SCREEN>(CPools::GetObjectRef(obj));
			lua_pushboolean(L, sw);
			return 1;
		}
		else { throw "bad argument in function isobjscreen"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ispeddead(lua_State* L) {// пед мертв.
	try {//
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			bool sw = Command<COMMAND_IS_CHAR_DEAD>(CPools::GetPedRef(ped));
			lua_pushboolean(L, sw);
			return 1;

		}
		else { throw "bad argument in function ispeddead"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int iscardead(lua_State* L) {// авто уничтожено.
	try {//
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.

			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			bool sw = Command<COMMAND_IS_CAR_DEAD>(CPools::GetVehicleRef(car));
			lua_pushboolean(L, sw);
			return 1;
		}
		else { throw "bad argument in function iscardead"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_follow_ped(lua_State* L) {// пед следует за педом.
	try {// 01D2: actor 0@ follow_player $PLAYER_CHAR
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			p = lua_topointer(L, 1);
			CPed* ped1 = findpedinpool(p);// получить указатель на педа.

			CPed* player = FindPlayerPed();// найти педа
			bool cheack;
			if (ped1 != player) {
				cheack = Command<COMMAND_SET_CHAR_OBJ_GOTO_CHAR_ON_FOOT>(CPools::GetPedRef(ped), CPools::GetPedRef(ped1));
				lua_pushboolean(L, cheack);
				return 1;
			}
			else {
				cheack = Command<COMMAND_SET_CHAR_OBJ_GOTO_PLAYER_ON_FOOT>(CWorld::PlayerInFocus, CPools::GetPedRef(ped1));
				lua_pushboolean(L, cheack);// отправить булевое значение.
				return 1;
			}

		}
		else { throw "bad argument in function ped_follow_ped"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_cars_damaged(lua_State* L) {// Все авто повреждены.
	try {// 03F4: set_all_vehicles_apply_damage_rules 0
		if (LUA_TNUMBER == lua_type(L, 1)) {
			int status = lua_tointeger(L, 1);// если число

			Command<COMMAND_SET_ALL_CARS_CAN_BE_DAMAGED>(status);
			return 0;
		}
		else { throw "bad argument in function set_cars_damaged"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_ped_targetted(lua_State* L) {// запрет целиться в педа.
	try {//	0568: set_actor $2291 untargetable 1
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на авто.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			int s = lua_tonumber(L, 2);// да или нет.
			Command<COMMAND_SET_CHAR_NEVER_TARGETTED>(CPools::GetPedRef(ped), s);
			return 0;
		}
		else { throw "bad argument in function set_ped_targetted"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_ped_friend(lua_State* L) {// уст дружественное отношения педа.
	try {//	04F5: set_actor $2291 as_player_friend $PLAYER_CHAR flag 1
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на авто.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			int s = lua_tonumber(L, 2);// да или нет.
			Command<COMMAND_SET_CHAR_AS_PLAYER_FRIEND>(CPools::GetPedRef(ped), s);
			return 0;
		}
		else { throw "bad argument in function set_ped_friend"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_ped_running(lua_State* L) {// пед может бежать.
	try {//0319: set_actor $2291 running 1
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на авто.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			int s = lua_tonumber(L, 2);// да или нет.
			Command<COMMAND_SET_CHAR_RUNNING>(CPools::GetPedRef(ped), s);
			return 0;
		}
		else { throw "bad argument in function set_ped_running"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_ped_damaged_gang(lua_State* L) {// уст педа уязвимым для членов банды.
	try {//0514: set_actor $2293 can_be_damaged_by_members_of_gang 3 0
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)
			&& LUA_TNUMBER == lua_type(L, 2)) {// указатель на педа.
			/*Делает персонажа уязвимым для членов банды
Синтаксис
0514: set_actor [идентификатор символа ] can_be_damaged_by_members_of_gang [ int1 ] [ int2 ]
Параметр [ дескриптор символа ] Ручка персонажа [ int1 ]
Номер банды [ int2 ]
0 = невозможно повредить, 1 = можно повредить (по умолчанию)
Этот код операции позволяет персонажу быть невосприимчивым к огнестрельному оружию со стороны членов банды,
в том числе любых персонажей, порожденных как пед-тип этой банды. Персонаж все еще может быть поврежден кулаками и эффектами области.
Из-за ошибки все персонажи постоянно невосприимчивы к огнестрельному оружию членов неиспользуемой банды номер 8 (тип педа GANG9).

			*/
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			int s = lua_tonumber(L, 2);//
			int s1 = lua_tonumber(L, 3);//

			Command<COMMAND_SET_CHAR_CAN_BE_DAMAGED_BY_MEMBERS_OF_GANG>(CPools::GetPedRef(ped), s, s1);
			return 0;
		}
		else { throw "bad argument in function set_ped_damaged_gang"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int is_ped_damaged_weapon(lua_State* L) {// пед получает от определенного вида оружие.
	try {//031D: actor $2291 hit_by_weapon 22
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на авто.
			/*
				Описание Проверяет, был ли персонаж поврежден указанным оружием или средой
				Синтаксис	031D: актер [ дескриптор символа ] hit_by_weapon [ int ]
				Параметр	[ дескриптор символа ]
				Ручка персонажа		[ int ]
				Тип оружия (или тип урона окружающей среды, указанный ниже)
				Родной аналог		HAS_CHAR_BEEN_DAMAGED_BY_WEAPON
				Этот условный код операции возвращает истину, если персонаж был поврежден указанным оружием или окружающей средой.
				Последний урон, нанесенный персонажу, можно очистить с помощью кода операции 0467 . Значения за пределами диапазона
				типов оружия поддерживаются для различных повреждений окружающей среды. Типы оружия для снарядов не распознаются,
				поэтому вместо них следует использовать типы повреждений окружающей среды.

				Значения экологического ущерба
				GTA III	Vice City	Enum	Примечания
				-	28	WEAPONTYPE_SNIPERRIFLE	код операции 0321 или 0322 (разнести голову)
				9	31 год	WEAPONTYPE_FLAMETHROWER	любой огонь, а не только огнемет
				16	39	WEAPONTYPE_RAMMEDBYCAR	поврежден, но не убит автомобилем
				17	40	WEAPONTYPE_RUNOVERBYCAR	мгновенно убит автомобилем, включает лопасти вертолета
				18	41 год	WEAPONTYPE_EXPLOSION	взрыв
				19	42	WEAPONTYPE_UZI_DRIVEBY	проезжающий мимо
				20	43	WEAPONTYPE_DROWNING	вода
				21 год	44	WEAPONTYPE_FALL	повреждение земли
				-	46	WEAPONTYPE_ANYMELEE	любой рукопашный бой
				-	47	WEAPONTYPE_ANYWEAPON	любое оружие (рукопашный бой, огонь, ружье, взрыв)

			*/
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			int s = lua_tonumber(L, 2);// да или нет.
			bool check = Command<COMMAND_HAS_CHAR_BEEN_DAMAGED_BY_WEAPON>(CPools::GetPedRef(ped), s);
			lua_pushboolean(L, check);
			return 1;
		}
		else { throw "bad argument in function is_ped_damaged_weapon"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int is_car_damaged_weapon(lua_State* L) {// авто получает от определенного вида оружие.
	try {//031E: vehicle $2467 hit_by_weapon 39
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на авто.
			/*
			Описание	Возвращает истину, если указанная машина повреждена указанным оружием.
Параметры (v2)	1) Ручка машины (целое число) (INT)
2) ID оружия (целое число) (INT)
			*/
			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			int s = lua_tonumber(L, 2);// да или нет.
			bool check = Command<COMMAND_HAS_CAR_BEEN_DAMAGED_BY_WEAPON>(CPools::GetVehicleRef(car), s);
			lua_pushboolean(L, check);
			return 1;
		}
		else { throw "bad argument in function is_car_damaged_weapon"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int isped_in_air(lua_State* L) {// пед в воздухе.
	try {//
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			bool sw = ped->CheckIfInTheAir();
			lua_pushboolean(L, sw);
			return 1;

		}
		else { throw "bad argument in function isped_in_air"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_threat_for_ped_type(lua_State* L) {// уст враждебность типа педа к другим типам педа.
	try {// 03F1: pedtype 9 add_threat 1.
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {//числа.

			int t1 = lua_tointeger(L, 1);// тип педа агрессор.
			int t2 = lua_tointeger(L, 2);// жертва.

			Command<COMMAND_SET_THREAT_FOR_PED_TYPE>(t1, t2);
			return 0;
		}
		else { throw "bad argument in function set_threat_for_ped_type"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int clean_threat_for_ped_type(lua_State* L) {// Убрать враждебность типа педа к другим типам педа.
	try {//03F2: pedtype 9 remove_threat 1.
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {//числа.

			int t1 = lua_tointeger(L, 1);// тип педа агрессор.
			int t2 = lua_tointeger(L, 2);// жертва.

			Command<COMMAND_CLEAR_THREAT_FOR_PED_TYPE>(t1, t2);
/* 03F2: pedtype 4 remove_threat 1048576
3)...PROFIT! UPD:
Пардон, это не исправит проблему - педы лишь перестанут "бояться" стрельбы,
а от самих пуль по-прежнему отпрыгивают...*/

			return 0;
		}
		else { throw "bad argument in function clean_threat_for_ped_type"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int create_phone(lua_State* L) {// создать телефон на координатах.
	try {// 024A: $292 = get_phone_at 36.90385 -1023.3
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {//числа.

			float x = lua_tonumber(L, 1);// координата x.
			float y = lua_tonumber(L, 2);// координата y.

/*Получает таксофон в точке координат
Синтаксис 024A: [ var ] = get_phone_at [ flt1 ] [ flt2 ]
[ var ] Переменная для хранения ручки телефона
[ flt1 ] X-координата
[ flt2 ] Координата Y
Этот код операции получает объект таксофона, соответствующий названию модели phonebooth1, который может быть помещен в мир через файл IPL,
ближайший к точке координат, и назначает ему дескриптор. В игре можно сохранить до 49 телефонов. Если таксофон уже взят, он берет
следующий ближайший таксофон. Сам по себе этот код операции не создает объект таксофона. Точка с координатами не должна быть точной,
но ее расстояние от телефона-автомата должно быть в пределах 100 единиц, иначе игра может вылететь.*/
			int fhone;// id телефона.
			Command<COMMAND_GRAB_PHONE>(x, y, fhone);
			lua_pushinteger(L, fhone);
			return 1;
		}
		else { throw "bad argument in function create_phone"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int on_phone(lua_State* L) {// вкл телефон.
	try {// 0405: enable_phone $292
		if (LUA_TNUMBER == lua_type(L, 1)) {//числа.

			int phone = lua_tonumber(L, 1);// координата x.
/* Включает таксофон
0405: enable_phone [ телефонный номер ]
Параметр [ телефонный номер ]
Ручка телефона Этот код операции включает таксофон путем звонка и увеличения движения объекта таксофона.
Телефон можно выключить с помощью кода операции 024E. */
			Command<COMMAND_TURN_PHONE_ON>(phone);
			return 0;
		}
		else { throw "bad argument in function on_phone"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int off_phone(lua_State* L) {// выкл телефон.
	try {//024E: disable_phone $292 Выключает таксофон
		if (LUA_TNUMBER == lua_type(L, 1)) {//числа.

			int phone = lua_tonumber(L, 1);// id телефона.
/*  024E: disable_phone [ телефонный номер ]
Параметр [ телефонный номер ]
Ручка телефона Этот код операции выключает телефон-автомат. Все эффекты, связанные с активным телефоном-автоматом,
удаляются: звонки, чрезмерные движения и любые телефонные сообщения.*/
			Command<COMMAND_TURN_PHONE_OFF>(phone);
			return 0;
		}
		else { throw "bad argument in function off_phone"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int read_memory(lua_State* L) {// читать адрес памяти.
	try {// 0A8D: $result = read_memory 1@ size 4 virtual_protect 0

		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {//числа.
			int address = lua_tointeger(L, 1);// адрес памяти.
			int size = lua_tointeger(L, 2);// размер ячейки памяти.
			if (size == 1)	{
				char res = patch::GetChar(address);
				lua_pushinteger(L, res);
				return 1;
			};

			if (size == 2)	{
				int res1 = patch::GetInt(address);
				lua_pushinteger(L, res1);
				return 1;
			};

			if (size == 4)	{
				float res2 = patch::GetFloat(address);
				lua_pushnumber(L, res2);
				return 1;
			};
		}
		else { throw "bad argument in function read_memory"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int write_memory(lua_State* L) {// записать адрес памяти.
	try {//0A8C: write_memory 0xC0BC15 size 1 value 1 virtual_protect 0

		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {//числа.
			int address = lua_tointeger(L, 1);// адрес памяти.
			int size = lua_tointeger(L, 2);// размер ячейки памяти.
			int value = lua_tointeger(L, 3);// значение памяти.
			if (size == 1)	{
				patch::SetChar(address, value);
				return 0;
			};

			if (size == 2)	{
				patch::SetShort(address, value);
				return 0;
			};

			if (size == 4)	{
				float value1 = lua_tonumber(L, 3);// значение памяти.
				patch::SetFloat(address, value1);
				return 0;
			};
		}
		else { throw "bad argument in function write_memory"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int is_car_model(lua_State* L) {// проверить на конкретную модель авто.
	try {// 0137: car $5651 id == #YARDIE
		if (LUA_TSTRING == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {// строка и указатель на авто.
			const char* name_model = lua_tostring(L, 1);// модель авто.

			int model = find_in_map(car_model_list, name_model);
			const void* p = lua_topointer(L, 2);

			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			bool check = Command<COMMAND_IS_CAR_MODEL>(CPools::GetVehicleRef(car), model);
			lua_pushboolean(L, check);
			return 1;
			}// int

		else { throw "bad argument in function is_car_model"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_player_control(lua_State* L) {// выкл телефон.
	try {//01B4: set_player $player_char can_move 1
		if (LUA_TNUMBER == lua_type(L, 1)) {//числа.

			int mod = lua_tonumber(L, 1);// координата x.

			Command<COMMAND_SET_PLAYER_CONTROL>(mod);
			return 0;

			/*	Устанавливает контроль над персонажем игрока 01B4: set_player[игрок ручка] can_move[INT]
	Параметр [ручка игрока]	Ручка игрока [int]	0 = неконтролируемый, 1 = управляемый(по умолчанию)
	SET_PLAYER_CONTROL
	Этот код операции устанавливает контроль над персонажем игрока.Отключение управления предотвратит влияние любых входных сигналов
	на персонажа игрока и камеру, которая следует за персонажем игрока.Многие эффекты в окружающей среде происходят, когда персонаж
	заморожен, в том числе тушение всех пожаров, игнорирование всеми пешеходами игрока и мгновенная остановка транспортного средства
	игрока, если игрок ведет машину.
	Command<COMMAND_SET_PLAYER_CONTROL>(CWorld::PlayerInFocus, angle);
			*/
		}
		else { throw "bad argument in function set_player_control"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};
int is_ped_sitting_in_car(lua_State* L) {// Возвращает истину, если пед находится в указанной машине.
	try {// 0448: actor $6312 sitting_in_car $6313.
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {//число.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.

			const void* p1 = lua_topointer(L, 2);
			CVehicle* car = findcarinpool(p1);//  получить указатель на авто.
			bool check = Command<COMMAND_IS_CHAR_SITTING_IN_CAR>(CPools::GetPedRef(ped), CPools::GetVehicleRef(car)); //пед садится в авто как водитель.
			lua_pushboolean(L, check);
			return 1;
		}
		else { throw "bad argument in function is_ped_sitting_in_car"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int is_sphere_onscreen(lua_State* L) {// видна точка на радаре.
	try {// 00C2: sphere_onscreen $6330 $6331 $6332 3.0 .
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {
			float x = lua_tonumber(L, 1);  float y = lua_tonumber(L, 2);
			float z = lua_tonumber(L, 3);  float radius = lua_tonumber(L, 4);// радиус.
			CVector pos = { x, y, z };
			bool check = Command<COMMAND_IS_POINT_ON_SCREEN>(pos.x, pos.y, pos.z, radius); // пед садится в авто как водитель.
			lua_pushboolean(L, check);
			return 1;
			/*	Возвращает истину, если на экране видна любая часть радиуса указанной точки.
             Полезно для тестирования, когда игрок находится или не смотрит на что-то.	*/
		}
		else { throw "bad argument in function is_sphere_onscreen "; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_ped_can_shot_in_car(lua_State* L) {// водитель авто может получить пули.
	try {// 054A: set_actor $6312 can_be_shot_in_a_car 0
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на авто.
			/*
Описание	Используется для предотвращения повреждений персонажа в транспортном средстве.
Параметры (v2)	1) Дескриптор актера (Целое число) (INT)
2) Логическое значение (1 или 0) (INT)
			*/
			const void* p = lua_topointer(L, 1);

			CPed* ped = findpedinpool(p);// получить указатель на педа.
			int s = lua_tointeger(L, 2);// да или нет.
			 Command<COMMAND_SET_CHAR_CAN_BE_SHOT_IN_VEHICLE>(CPools::GetPedRef(ped), s);
			return 0;
		}
		else { throw "bad argument in function set_ped_can_shot_in_car"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int iscarfire(lua_State* L) {// авто горит?
	try {// 0495: vehicle $6313 burning
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.

			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			bool sw = Command<COMMAND_IS_CAR_ON_FIRE>(CPools::GetVehicleRef(car));
			lua_pushboolean(L, sw);
			return 1;
		}
		else { throw "bad argument in function iscarfire"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

bool check_segment(float x1_1, float y1_1, float x1_2, float y1_2, float x2_1, float y2_1, float x2_2,  float y2_2) {

	float Ua, Ub, numerator_a, numerator_b, denominator;
	denominator = (y2_2 - y2_1) * (x1_1 - x1_2) - (x2_2 - x2_1) * (y1_1 - y1_2);
	if (denominator == 0) {
		if ((x1_1 * y1_2 - x1_2 * y1_1) * (x2_2 - x2_1) - (x2_1 * y2_2 - x2_2 * y2_1) * (x1_2 - x1_1) == 0 && (x1_1 * y1_2 - x1_2 * y1_1)
			* (y2_2 - y2_1) - (x2_1 * y2_2 - x2_2 * y2_1) * (y1_2 - y1_1) == 0) {

			return true;
		}
		else {
			return false;
		}
	}
	else {
		numerator_a = (x2_2 - x1_2) * (y2_2 - y2_1) - (x2_2 - x2_1) * (y2_2 - y1_2);
		numerator_b = (x1_1 - x1_2) * (y2_2 - y1_2) - (x2_2 - x1_2) * (y1_1 - y1_2);
		Ua = numerator_a / denominator;
		Ub = numerator_b / denominator;
		if (Ua >= 0 && Ua <= 1 && Ub >= 0 && Ub <= 1) {
			return true;
		}
		else {
			return false;
		}
	}

};

int is_car_damage_by_car(lua_State* L) {// Если авто получило урон от другого авто.
	try {// 051D: car 57@ damaged_by_car 40@

		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {//указатель на авто.

			const void* p = lua_topointer(L, 1);
			const void* p1 = lua_topointer(L, 2);
			//Command<COMMAND_ADD_SPHERE>(pos.x, pos.y, 10.0, 0.6, &sphere); //создать, удалить, создать сферу
			//Command<COMMAND_REMOVE_SPHERE>(sphere);// нужно, чтобы обойти глюк.
			//Command<COMMAND_ADD_SPHERE>(pos.x, pos.y, 10.0, 0.6, &sphere);
			//this_thread::sleep_for(chrono::milliseconds(10));
			int sphere;// переменная, которая хранить id сферы.

			CVehicle* car = findcarinpool(p);//  получить указатель на авто.
			CVehicle* car1 = findcarinpool(p1);//  получить указатель на авто.

			vector<vector<float>>car_cord;// {// 1{2},3{2},3{1},1{1 } };
			vector<vector<float>>car_cord1;
			car_cord.resize(4, vector<float>(4));
			car_cord1.resize(4, vector<float>(4));

			CVector pos;
			float length, width, height, x, x1, y, y1, x_end, x1_end, y_end, y1_end;

			pos = car->m_pHandlingData->m_vDimensions;
			length = pos.x; width = pos.y; height = pos.z;

			length = +0.98;	width = (width + 0.64);// / 2;

			// первого авто.
			pos = car->pos + car->right * width + car->up * length;
			car_cord[0][0] = pos.x; car_cord[0][1] = pos.y;//правый верхний угол первого авто.

			pos = car->pos + car->right * (width * -1) + car->up * length;
			car_cord[0][2] = pos.x; car_cord[0][3] = pos.y;//левый верхний угол первого авто.
			// 1 сторона
			//car[0][0] = x; 	car[0][1] = y;  car[0][2] = x_end; car[0][3] = y_end;

			car_cord[1][0] = pos.x; car_cord[1][1] = pos.y;

			pos = car->pos + car->right * (width * -1) + car->up * (length * -1);
			car_cord[1][2] = pos.x; car_cord[1][3] = pos.y;	;//левый нижний угол первого авто.
			// 2 сторона. 	//car[1][0] = 3; car[1][1] = 2;  car[1][2] = 3; car[1][3] = 1;

			car_cord[2][0] = pos.x; car_cord[2][1] = pos.y;

			pos = car->pos + car->right * width + car->up * (length * -1);
			car_cord[2][2] = pos.x; car_cord[3][3] = pos.y;//правый нижний угол первого авто.
			// 3 сторона //car[2][0] = 3; car[2][1] = 1;  car[2][2] = 1; car[3][3] = 1;

			car_cord[3][0] = pos.x; car_cord[3][1] = pos.y;

			pos = car->pos + car->right * width + car->up * length;
			car_cord[3][2] = pos.x; car_cord[3][3] = pos.y;//правый верхний угол первого авто.
			// 4 сторона.
			//car[3][0] = 1; car[3][1] = 1;  car[3][2] = 1; car[3][3] = 2;//координанты вершин 1 прямоугольника.

			// второе авто.
			pos = car1->m_pHandlingData->m_vDimensions; 	length = pos.x; width = pos.y; height = pos.z;

			length = +0.98;	width = (width + 0.64);// / 2;

			pos = car1->pos + car1->right * width + car1->up * length;
			car_cord1[0][0] = pos.x; car_cord1[0][1] = pos.y;//правый верхний угол второго авто.

			pos = car1->pos + car1->right * (width * -1) + car1->up * length;
			car_cord1[0][2] = pos.x; car_cord1[0][3] = pos.y;//левый верхний угол второго авто.
			// 1 сторона
			//car[0][0] = 1; 	car[0][1] = 2;  car[0][2] = 3; car[0][3] = 2;

			car_cord1[1][0] = pos.x; car_cord1[1][1] = pos.y;

			pos = car1->pos + car1->right * (width * -1) + car1->up * (length * -1);
			car_cord1[1][2] = pos.x; car_cord1[1][3] = pos.y; ;//левый нижний угол второго авто.
			// 2 сторона. 	//car[1][0] = 3; car[1][1] = 2;  car[1][2] = 3; car[1][3] = 1;

			car_cord1[2][0] = pos.x; car_cord1[2][1] = pos.y;

			pos = car1->pos + car1->right * width + car1->up * (length * -1);
			car_cord1[2][2] = pos.x; car_cord1[3][3] = pos.y;//правый нижний угол второго авто.
			// 3 сторона //car[2][0] = 3; car[2][1] = 1;  car[2][2] = 1; car[3][3] = 1;

			car_cord1[3][0] = pos.x; car_cord1[3][1] = pos.y;

			pos = car1->pos + car1->right * width + car1->up * length;
			car_cord1[3][2] = pos.x; car_cord1[3][3] = pos.y;//правый верхний угол второго авто.
			// 4 сторона.
			//car[3][0] = 1; car[3][1] = 1;  car[3][2] = 1; car[3][3] = 2;//координанты вершин 2 прямоугольника.


			for (int i = 0; i < 4; i++) {
				x = car_cord[i][0];	y = car_cord[i][1];	x_end = car_cord[i][2];	y_end = car_cord[i][3];
				for (int i = 0; i < 4; i++) {
					x1 = car_cord1[i][0];	y1 = car_cord1[i][1];	 x1_end = car_cord1[i][2];	y1_end = car_cord1[i][3];

					if (check_segment(x, y, x_end, y_end, x1, y1, x1_end, y1_end) && car->m_fCollisionPower > 0 && car1->m_fCollisionPower > 0
						&& car->GetHasCollidedWith(car1)
						) {
						lua_pushboolean(L, true);
							return 1;
					}
				}
			};

			lua_pushboolean(L, false);
			return 1;

						//&& 39 == car->m_nLastWeaponDamage && 39 == car->m_nLastWeaponDamage
		}
		else { throw "bad argument in function is_car_damage_by_car"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int show_text_gtx_table(lua_State* L) {// вывести текст из таблицы.
	try {
		if (LUA_TSTRING == lua_type(L, 1) && LUA_TSTRING == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)
			&& LUA_TNUMBER == lua_type(L, 4)) {// значение число.
			const char* table = lua_tostring(L, 1);// таблица.
			const char* text = lua_tostring(L, 2);// текст.
			int time = lua_tointeger(L, 3);	int type = lua_tointeger(L, 4);
			Command<COMMAND_LOAD_MISSION_TEXT>(table);
			Command<COMMAND_PRINT_NOW>(text, time, type);// .
			lua_settop(L, 0);// очистить стек.
			return 0;
		}

		else { throw "bad argument in function show_text_gtx_table"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int load_gtx_table(lua_State* L) {// вывести текст из таблицы.
	try {
		if (LUA_TSTRING == lua_type(L, 1)) {// значение число.
			const char* table = lua_tostring(L, 1);// таблицу.
			Command<COMMAND_LOAD_MISSION_TEXT>(table);
			lua_settop(L, 0);// очистить стек.
			return 0;
		}

		else { throw "bad argument in function load_gtx_table"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_road_off (lua_State* L) {// выкл путь педов.
	try {// 022B: create_forbidden_for_peds_cube 354.9 - 483.1 21.0 406.0 - 490.0 10.0
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)
			&& LUA_TNUMBER == lua_type(L, 4) && LUA_TNUMBER == lua_type(L, 5) && LUA_TNUMBER == lua_type(L, 6)) {

			float x= lua_tointeger(L, 1); float y = lua_tonumber(L, 2);	float z = lua_tonumber(L, 3);
			float x1 = lua_tonumber(L, 4);	float y1 = lua_tonumber(L, 5); float z1 = lua_tonumber(L, 6);

			Command<COMMAND_SWITCH_PED_ROADS_OFF>(x, y, z, x1, y1, z1);
				/*Описание	Запрещает пешеходам входить в указанную зону.
					Параметры (v2)	1) X Coord (Float) (FLOAT)
					2) Y Coord (Float) (FLOAT)
					3) Z Coord (Float) (FLOAT)
					4) X Coord (Float) (FLOAT)
					5) Координата Y (Float) (FLOAT)
					6) Координата Z (Плавающая) (Плавающая)
					sascm.ini	022B = 6, switch_ped_roads_off% 1d%% 2d%% 3d% до% 4d%% 5d%% 6d%
					*/
				return 0;
		}
		else { throw "bad argument in function ped_road_off"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_road_on(lua_State* L) {// вкл путь педов.
	try {//	022A: remove_forbidden_for_peds_cube 354.9 - 483.1 21.0 406.0 - 490.0 0.0

		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)
			&& LUA_TNUMBER == lua_type(L, 4) && LUA_TNUMBER == lua_type(L, 5) && LUA_TNUMBER == lua_type(L, 6)) {

			float x = lua_tointeger(L, 1); float y = lua_tonumber(L, 2);	float z = lua_tonumber(L, 3);
			float x1 = lua_tonumber(L, 4);	float y1 = lua_tonumber(L, 5); float z1 = lua_tonumber(L, 6);

			Command<COMMAND_SWITCH_PED_ROADS_ON>(x, y, z, x1, y1, z1);
			return 0;
		}
		else { throw "bad argument in function ped_road_on"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int car_road_off(lua_State* L) {// выкл путь авто.
	try {// 01E8: create_forbidden_for_cars_cube -291.0 -287.0 0.0 208.0 648.0 35.0

		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)
			&& LUA_TNUMBER == lua_type(L, 4) && LUA_TNUMBER == lua_type(L, 5) && LUA_TNUMBER == lua_type(L, 6)) {

			float x = lua_tointeger(L, 1); float y = lua_tonumber(L, 2);	float z = lua_tonumber(L, 3);
			float x1 = lua_tonumber(L, 4);	float y1 = lua_tonumber(L, 5); float z1 = lua_tonumber(L, 6);
			Command<COMMAND_SWITCH_ROADS_OFF>(x, y, z, x1, y1, z1);	//	Предотвращает движение автомобилей по дорогам в указанной зоне.

			return 0;
		}
		else { throw "bad argument in function car_road_off"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int car_road_on(lua_State* L) {// вкл путь авто.
	try {//01E7: remove_forbidden_for_cars_cube -291.0 -287.0 0.0 208.0 648.0 35.0

		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)
			&& LUA_TNUMBER == lua_type(L, 4) && LUA_TNUMBER == lua_type(L, 5) && LUA_TNUMBER == lua_type(L, 6)) {

			float x = lua_tointeger(L, 1); float y = lua_tonumber(L, 2);	float z = lua_tonumber(L, 3);
			float x1 = lua_tonumber(L, 4);	float y1 = lua_tonumber(L, 5); float z1 = lua_tonumber(L, 6);

			Command<COMMAND_SWITCH_ROADS_ON>(x, y, z, x1, y1, z1);
			return 0;
		}
		else { throw "bad argument in function car_road_on"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int findped_in_cord(lua_State* L) {// найти педа в радиусе с координатами.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)	&& LUA_TNUMBER == lua_type(L, 4)) {

				float x = lua_tonumber(L, 1); float y = lua_tonumber(L, 2);	float z = lua_tonumber(L, 3);
				float radius = lua_tonumber(L, 4); CVector pos = { x, y, z };// вектор координат.
				CPed* p = FindPlayerPed(); CVehicle* v = NULL;
    			if (p->m_bInVehicle && p->m_pVehicle != NULL) {// в авто пед?
				       v = p->m_pVehicle;	}// получить указатель на хенлд авто в котором сидит томии.

			for (auto car : CPools::ms_pVehiclePool) {
				if (car != v && CVector::Distance(car->GetPosition(), pos) < radius) {
					car->CanPedExitCar(true);
					if (CPed * p1 = car->m_pDriver) {
						if (p1 != NULL && p1 != p) {
							lua_pushboolean(L, true); lua_pushlightuserdata(L, p1);// отправить в стек и получить из стека можно
							return 2;
			}	}	}
			}
			for (auto ped : CPools::ms_pPedPool) {
				if (ped != p && CVector::Distance(ped->GetPosition(), pos) < radius) {
					lua_pushboolean(L, true); lua_pushlightuserdata(L, ped);// отправить в стек и получить из стека можно
					return 2;
				}
			}//
			CPed* p2 = NULL; lua_pushboolean(L, false);
			lua_pushlightuserdata(L, p2);// отправить в стек и получить из стека можно
			return 2;
		}
		else { throw "bad argument in function findped_in_cord"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int findcar_in_cord(lua_State* L) {// найти авто в радиусе с координатами.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)	&& LUA_TNUMBER == lua_type(L, 4)) {

			float x = lua_tonumber(L, 1); float y = lua_tonumber(L, 2);	float z = lua_tonumber(L, 3);
			float radius = lua_tonumber(L, 4); CVector pos = { x, y, z };// вектор координат.
			CPed* p = FindPlayerPed(); CVehicle* v = NULL;

			if (p->m_bInVehicle && p->m_pVehicle != NULL) {// в авто пед?
				CVehicle* v = p->m_pVehicle;}// получить указатель на хенлд авто в котором сидит томии.
			for (auto car : CPools::ms_pVehiclePool) {
				if (car != v && CVector::Distance(car->GetPosition(), pos) < radius) {
					lua_pushboolean(L, true); lua_pushlightuserdata(L, car);// отправить в стек и получить из стека можно
					return 2;
				}	}
			CVehicle* car = NULL;
			lua_pushboolean(L, false); lua_pushlightuserdata(L, car);// отправить в стек и получить из стека можно
			return 2;
		}
		else { throw "bad argument in function findcar_in_cord"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int findobj_in_cord(lua_State* L) {// найти объект в радиусе с координатами.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {

			float x = lua_tonumber(L, 1); float y = lua_tonumber(L, 2);	float z = lua_tonumber(L, 3);
			float radius = lua_tonumber(L, 4);  const CVector pos = { x, y, z };// вектор координат.

			for (auto obj : CPools::ms_pObjectPool) {
				if (CVector::Distance(obj->GetPosition(), pos) < radius) {
					lua_pushboolean(L, true); lua_pushlightuserdata(L, obj);// отправить в стек и получить из стека можно
					return 2;
				}
			}//
			CObject* obj1 = NULL; lua_pushboolean(L, false);
			lua_pushlightuserdata(L, obj1);// отправить в стек и получить из стека можно
			return 2;
		}
		else { throw "bad argument in function findobj_in_cord"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int findped_nearest_in_cord(lua_State* L) {// найти ближайшего педа в радиусе с координатами.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {

			float x = lua_tointeger(L, 1); float y = lua_tonumber(L, 2); float z = lua_tonumber(L, 3);
			CVector pos = { x, y, z };// вектор координат.
			CPed* p = FindPlayerPed(); CVehicle* v = NULL;
			if (p->m_bInVehicle && p->m_pVehicle != NULL) {// в авто пед?
				v = p->m_pVehicle;	}// получить указатель на хенлд авто в котором сидит томии.
			for (float radius = 1.0; radius < 101.0; radius++) {

				for (auto car : CPools::ms_pVehiclePool) {
					if (car != v && CVector::Distance(car->GetPosition(), pos) < radius) {
						car->CanPedExitCar(true);
						if (CPed * p1 = car->m_pDriver) {
							if (p1 != NULL && p1 != p) {
								lua_pushboolean(L, true); lua_pushlightuserdata(L, p1);// отправить в стек и получить из стека можно
								return 2;}	}
					}
				}
				for (auto ped : CPools::ms_pPedPool) {
					if (ped != p && CVector::Distance(ped->GetPosition(), pos) < radius) {
						lua_pushboolean(L, true); lua_pushlightuserdata(L, ped);// отправить в стек и получить из стека можно
						return 2;
					}
				}//
			}
			CPed* p2 = NULL; lua_pushboolean(L, false);
			lua_pushlightuserdata(L, p2);// отправить в стек и получить из стека можно
			return 2;
		}
		else { throw "bad argument in function findped_nearest_in_cord"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int findcar_nearest_in_cord(lua_State* L) {// найти ближайшее авто в радиусе с координатами.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {

			float x = lua_tointeger(L, 1); float y = lua_tonumber(L, 2); float z = lua_tonumber(L, 3);	CVector pos = { x, y, z };// вектор координат.
			CPed* p = FindPlayerPed(); CVehicle* v = NULL;

			if (p->m_bInVehicle && p->m_pVehicle != NULL) {// в авто пед?
				CVehicle* v = p->m_pVehicle;	}// получить указатель на хенлд авто в котором сидит томии.

			for (float radius = 1.0; radius < 101.0; radius++) {
				for (auto car : CPools::ms_pVehiclePool) {
      			  if (car != v && CVector::Distance(car->GetPosition(), pos) < radius) {
					  lua_pushboolean(L, true); lua_pushlightuserdata(L, car);// отправить в стек и получить из стека можно
						return 2;	}	}	}
			CVehicle* car = NULL;
			lua_pushboolean(L, false); lua_pushlightuserdata(L, car);// отправить в стек и получить из стека можно
			return 2;
		}
		else { throw "bad argument in function findcar_nearest_in_cord"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int findobj_nearest_in_cord(lua_State* L) {// найти ближайший объект в радиусе с координатами.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {

		float x = lua_tointeger(L, 1); float y = lua_tonumber(L, 2); float z = lua_tonumber(L, 3); CVector pos = { x, y, z };// вектор координат.

			for (float radius = 1.0; radius < 101.0; radius++) {
				for (auto obj : CPools::ms_pObjectPool) {
					if (CVector::Distance(obj->GetPosition(), pos) < radius) {
						lua_pushboolean(L, true); lua_pushlightuserdata(L, obj);// отправить в стек и получить из стека можно
						return 2;	}
				}	}//
			CObject* obj1 = NULL; lua_pushboolean(L, false);
			lua_pushlightuserdata(L, obj1);// отправить в стек и получить из стека можно
			return 2;
		}
		else { throw "bad argument in function findobj_in_cord"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int check_obj_in_cord(lua_State* L) {// есть ли объект в радиусе с координатами.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {

			float x = lua_tonumber(L, 1); float y = lua_tonumber(L, 2);	float z = lua_tonumber(L, 3);
			float radius = lua_tonumber(L, 4);  const CVector pos = { x, y, z };// вектор координат.

			for (auto obj : CPools::ms_pObjectPool) {
				if (CVector::Distance(obj->GetPosition(), pos) < radius) {
					lua_pushboolean(L, true); // отправить в стек и получить из стека можно
					return 1;
				}
			}//
			lua_pushboolean(L, false);// отправить в стек и получить из стека можно
			return 1;
		}
		else { throw "bad argument in function check_obj_in_cord"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int get_obj_in_cord(lua_State* L) {// получить объект в радиусе с координатами.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {

			float x = lua_tonumber(L, 1); float y = lua_tonumber(L, 2);	float z = lua_tonumber(L, 3);
			float radius = lua_tonumber(L, 4);  const CVector pos = { x, y, z };// вектор координат.

			for (auto obj : CPools::ms_pObjectPool) {
				if (CVector::Distance(obj->GetPosition(), pos) < radius) {
					lua_pushlightuserdata(L, obj);// отправить в стек и получить из стека можно
					return 1;
				}
			}//
			CObject* obj1 = NULL;
			lua_pushlightuserdata(L, obj1);// отправить в стек и получить из стека можно
			return 1;
		}
		else { throw "bad argument in function get_obj_in_cord"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int check_ped_in_cord(lua_State* L) {// есть ли педа в радиусе с координатами.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {

			float x = lua_tonumber(L, 1); float y = lua_tonumber(L, 2);	float z = lua_tonumber(L, 3);
			float radius = lua_tonumber(L, 4); CVector pos = { x, y, z };// вектор координат.
			CPed* p = FindPlayerPed(); CVehicle* v = NULL;
			if (p->m_bInVehicle && p->m_pVehicle != NULL) {// в авто пед?
				v = p->m_pVehicle;	}// получить указатель на хенлд авто в котором сидит томии.

			for (auto car : CPools::ms_pVehiclePool) {
				if (car != v && CVector::Distance(car->GetPosition(), pos) < radius) {
					car->CanPedExitCar(true);
					if (CPed * p1 = car->m_pDriver) {
						if (p1 != NULL && p1 != p) {
							lua_pushboolean(L, true);// отправить в стек и получить из стека можно
							return 1;
						}
					}
				}
			}
			for (auto ped : CPools::ms_pPedPool) {
				if (ped != p && CVector::Distance(ped->GetPosition(), pos) < radius) {
					lua_pushboolean(L, true); // отправить в стек и получить из стека можно
					return 1;
				}
			}//
			CPed* p2 = NULL; lua_pushboolean(L, false);
			lua_pushlightuserdata(L, p2);// отправить в стек и получить из стека можно
			return 2;
		}
		else { throw "bad argument in function check_ped_in_cord"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int get_ped_in_cord(lua_State* L) {// получить педа в радиусе с координатами.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {

			float x = lua_tonumber(L, 1); float y = lua_tonumber(L, 2);	float z = lua_tonumber(L, 3);
			float radius = lua_tonumber(L, 4); CVector pos = { x, y, z };// вектор координат.
			CPed* p = FindPlayerPed(); CVehicle* v = NULL;
			if (p->m_bInVehicle && p->m_pVehicle != NULL) {// в авто пед?
				v = p->m_pVehicle;	}// получить указатель на хенлд авто в котором сидит томии.

			for (auto car : CPools::ms_pVehiclePool) {
				if (car != v && CVector::Distance(car->GetPosition(), pos) < radius) {
					car->CanPedExitCar(true);
					if (CPed * p1 = car->m_pDriver) {
						if (p1 != NULL && p1 != p) {
							lua_pushboolean(L, true); lua_pushlightuserdata(L, p1);// отправить в стек и получить из стека можно
							return 2;
						}
					}
				}
			}
			for (auto ped : CPools::ms_pPedPool) {
				if (ped != p && CVector::Distance(ped->GetPosition(), pos) < radius) {
					lua_pushboolean(L, true); lua_pushlightuserdata(L, ped);// отправить в стек и получить из стека можно
					return 2;
				}
			}//
			CPed* p2 = NULL; lua_pushboolean(L, false);
			lua_pushlightuserdata(L, p2);// отправить в стек и получить из стека можно
			return 2;
		}
		else { throw "bad argument in function get_ped_in_cord"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int check_car_in_cord(lua_State* L) {// есть ли авто в радиусе с координатами.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {

			float x = lua_tonumber(L, 1); float y = lua_tonumber(L, 2);	float z = lua_tonumber(L, 3);
			float radius = lua_tonumber(L, 4); CVector pos = { x, y, z };// вектор координат.
			CPed* p = FindPlayerPed(); CVehicle* v = NULL;

			if (p->m_bInVehicle && p->m_pVehicle != NULL) {// в авто пед?
				CVehicle* v = p->m_pVehicle;	}// получить указатель на хенлд авто в котором сидит томии.
			for (auto car : CPools::ms_pVehiclePool) {
				if (car != v && CVector::Distance(car->GetPosition(), pos) < radius) {
					lua_pushboolean(L, true);// отправить в стек и получить из стека можно
					return 1;
				}
			}
				lua_pushboolean(L, false); // отправить в стек и получить из стека можно
			    return 1;
		}
		else { throw "bad argument in function check_car_in_cord"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int get_car_in_cord(lua_State* L) {// найти авто в радиусе с координатами.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {

			float x = lua_tonumber(L, 1); float y = lua_tonumber(L, 2);	float z = lua_tonumber(L, 3);
			float radius = lua_tonumber(L, 4); CVector pos = { x, y, z };// вектор координат.
			CPed* p = FindPlayerPed(); CVehicle* v = NULL;

			if (p->m_bInVehicle && p->m_pVehicle != NULL) {// в авто пед?
				CVehicle* v = p->m_pVehicle;}// получить указатель на хенлд авто в котором сидит томии.
			for (auto car : CPools::ms_pVehiclePool) {
				if (car != v && CVector::Distance(car->GetPosition(), pos) < radius) {
					lua_pushlightuserdata(L, car);// отправить в стек и получить из стека можно
					return 2;
				}
			}
			CVehicle* car = NULL;
			lua_pushlightuserdata(L, car);// отправить в стек и получить из стека можно
			return 1;
		}
		else { throw "bad argument in function get_car_in_cord"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int getcardamage(lua_State* L) { // получить кол-во урона авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.

			float damage = car->m_fCollisionPower; // получить кол-во урона авто.
			lua_pushnumber(L, damage);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function getcardamage"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int getmodelindex(lua_State* L) { // получить id модели.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);

			CEntity* p1 = (CEntity*)p;
			int modelindex = p1->m_nModelIndex; // получить id сущности.
			lua_pushinteger(L, modelindex);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function getmodelindex"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int getcardimension(lua_State* L) { // получить размер авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			//tHandlingData* data;
			float x, y, z;
			//CVector pos = { x, y, z };// вектор координат.
			//pos =data->m_vDimensions;
			CVector pos = car->m_pHandlingData->m_vDimensions;
			x = pos.x; y = pos.y; z = pos.z;
			lua_pushnumber(L, x);// отправить в стек.
			lua_pushnumber(L, y);// отправить в стек.
			lua_pushnumber(L, z);// отправить в стек.
			return 3;
		}
		else { throw "bad argument in function getcardimension"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int carmoveforce(lua_State* L) { // двигать авто с силой.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)
			&& LUA_TNUMBER == lua_type(L, 4)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);// получить указатель на авто.

			float x = lua_tonumber(L, 2);	float y = lua_tonumber(L, 3);		float z = lua_tonumber(L, 4);// координаты.

				car->ApplyMoveForce(x, y, z);

			return 0;
		}
		else { throw "bad argument in function carmoveforce"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int carturnforce(lua_State* L) { // повернуть авто с силой.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)
			&& LUA_TNUMBER == lua_type(L, 5) && LUA_TNUMBER == lua_type(L, 6) && LUA_TNUMBER == lua_type(L, 7)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);// получить указатель на авто.

			float x = lua_tonumber(L, 2);	float y = lua_tonumber(L, 3);
			float z = lua_tonumber(L, 4);	float rx = lua_tonumber(L, 5);	float ry = lua_tonumber(L, 6);
			float rz = lua_tonumber(L, 7);// координаты.

			car->ApplyTurnForce(x,y,z,rx,ry,rz);
				return 0;
		}
		else { throw "bad argument in function carturnforce"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int carturnfrictionforce(lua_State* L) { // повернуть авто с силой трения.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)
			&& LUA_TNUMBER == lua_type(L, 5) && LUA_TNUMBER == lua_type(L, 6) && LUA_TNUMBER == lua_type(L, 7)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);// получить указатель на авто.

			float x = lua_tonumber(L, 2);	float y = lua_tonumber(L, 3);
			float z = lua_tonumber(L, 4);	float rx = lua_tonumber(L, 5);	float ry = lua_tonumber(L, 6);
			float rz = lua_tonumber(L, 7);// координаты.

			car->ApplyFrictionTurnForce(x, y, z, rx, ry, rz);

			return 0;
		}
		else { throw "bad argument in function carturnfrictionforce"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int car_turn_speed(lua_State* L) { // повернуть авто с применением скорости.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) ) {// указатель на авто.

			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);// получить указатель на авто.
            car->ApplyTurnSpeed();

			return 0;
		}
		else { throw "bad argument in function car_turn_speed"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int car_move_speed(lua_State* L) { // двигать авто с применением скорости.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.

			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			car->ApplyMoveSpeed();

			//car->ApplyAirResistance();
			return 0;
		}
		else { throw "bad argument in function car_move_speed"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int car_turnforce_on_x(lua_State* L) {// Повернуть авто по оси x.
try {
	if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на авто.

			const void* p = lua_topointer(L, 1);

			float turn = lua_tonumber(L, 2);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.

			car->m_vecFrictionTurnForce.x = turn;
			return 0;
		}
		else { throw "bad argument in function car_turnforce_on_x"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int car_turnforce_on_y(lua_State* L) {// Повернуть авто по оси y.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на авто.

			const void* p = lua_topointer(L, 1);

			float turn = lua_tonumber(L, 2);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.

			car->m_vecFrictionTurnForce.y = turn;
			return 0;
		}
		else { throw "bad argument in function car_turnforce_on_y"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int car_turnforce_on_z(lua_State* L) {// Повернуть авто по оси z.
	static CVehicle* car;
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на авто.

			const void* p = lua_topointer(L, 1);

			float turn = lua_tonumber(L, 2);
			 car = findcarinpool(p);// получить указатель на авто.

			car->m_vecFrictionTurnForce.z = turn;// когда игрок сидит в ней может вращать.
			return 0;
		}
		else { throw "bad argument in function car_turnforce_on_z"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int car_moveforce_on_x(lua_State* L) {// Двигать авто по оси x.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на авто.

			const void* p = lua_topointer(L, 1);

			float turn = lua_tonumber(L, 2);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.

			car->m_vecFrictionMoveForce.x = turn;
			return 0;
		}
		else { throw "bad argument in function car_moveforce_on_x"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int car_moveforce_on_y(lua_State* L) {// Повернуть авто по оси y.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на авто.

			const void* p = lua_topointer(L, 1);

			float turn = lua_tonumber(L, 2);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.

			car->m_vecFrictionMoveForce.y = turn;
			return 0;
		}
		else { throw "bad argument in function car_moveforce_on_y"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int car_moveforce_on_z(lua_State* L) {// Повернуть авто по оси z.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на авто.

			const void* p = lua_topointer(L, 1);

			float turn = lua_tonumber(L, 2);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.

			car->m_vecFrictionMoveForce.z = turn;
			return 0;
		}
		else { throw "bad argument in function car_move_on_z"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int setcarrotate(lua_State* L) { // уст вращения авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)
			&& LUA_TNUMBER == lua_type(L, 4)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);// получить указатель на авто.

			float x = lua_tonumber(L, 2);	float y = lua_tonumber(L, 3);		float z = lua_tonumber(L, 4);// координаты.

			car->SetRotate(x, y, z);

			return 0;
		}
		else { throw "bad argument in function setcarrotate"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int carmovespeed(lua_State* L) { // двигать авто с скоростью.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)
			&& LUA_TNUMBER == lua_type(L, 4)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);// получить указатель на авто.

			float x = lua_tonumber(L, 2);	float y = lua_tonumber(L, 3);		float z = lua_tonumber(L, 4);// координаты.

			car->m_vecMoveSpeed.x = x; car->m_vecMoveSpeed.y = y;
			car->m_vecMoveSpeed.z = z;
			return 0;
		}
		else { throw "bad argument in function carmovespeed"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int carmovespeed_on_x(lua_State* L) { // двигать авто с скоростью по x.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);// получить указатель на авто.

			float x = lua_tonumber(L, 2);

			car->m_vecMoveSpeed.x = x;
			return 0;
		}
		else { throw "bad argument in function carmovespeed_on_x"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int carmovespeed_on_y(lua_State* L) { // двигать авто с скоростью по y.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);// получить указатель на авто.

			float y = lua_tonumber(L, 2);

			car->m_vecMoveSpeed.y = y;
			return 0;
		}
		else { throw "bad argument in function carmovespeed_on_y"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int carmovespeed_on_z(lua_State* L) { // двигать авто с скоростью по z.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			float z = lua_tonumber(L, 2);

			car->m_vecMoveSpeed.z = z;
			return 0;
		}
		else { throw "bad argument in function carmovespeed_on_z"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int car_rotate_on_x(lua_State* L) {// двигать авто по оси x.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			float x = lua_tonumber(L, 2);
			car->RotateX(x);
			return 0;
		}
		else { throw "bad argument in function car_rotate_on_x"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int car_rotate_on_y(lua_State* L) {// двигать авто по оси y.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			float y = lua_tonumber(L, 2);
			car->RotateY(y);
			return 0;
		}
		else { throw "bad argument in function car_rotate_on_y"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int car_rotate_on_z(lua_State* L) {// двигать авто по оси z.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			float z = lua_tonumber(L, 2);
			car->RotateZ(z);
			return 0;
		}
		else { throw "bad argument in function car_rotate_on_z"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int car_setrotate_on_x(lua_State* L) {// уст авто по оси x.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			float x = lua_tonumber(L, 2);
			car->SetRotateX(x);
			return 0;
		}
		else { throw "bad argument in function car_setrotate_on_x"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int car_setrotate_on_y(lua_State* L) {// уст авто по оси y.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			float y = lua_tonumber(L, 2);
			car->SetRotateY(y);
			return 0;
		}
		else { throw "bad argument in function car_setrotate_on_x"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int car_setrotate_on_z(lua_State* L) {// уст авто по оси z.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			float z = lua_tonumber(L, 2);
			car->SetRotateZ(z);
			return 0;
		}
		else { throw "bad argument in function car_setrotate_on_z"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int getcar_resistance(lua_State* L) {// Получить сопротивления авто трению.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.

			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);// получить указатель на авто.

			int res = int(car) + 0x0bc; //	turn resistance
			float res1 = patch::GetFloat(res);
			lua_pushnumber(L, res1);
			return 1;
		}
		else { throw "bad argument in function getcar_resistance"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int car_rotate_turn_on_x(lua_State* L) {// уст угол поворота авто по оси x.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			double speed = lua_tonumber(L, 2);
			double turn = lua_tonumber(L, 3);

			car->m_vecTurnSpeed.x = speed;
			car->m_vecFrictionTurnForce.x = turn;

			return 0;
		}
		else { throw "bad argument in function car_rotate_turn_on_x"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int car_rotate_turn_on_y(lua_State* L) {// уст угол поворота авто по оси y.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			double speed = lua_tonumber(L, 2);
			double turn = lua_tonumber(L, 3);

			car->m_vecTurnSpeed.y = speed;
			car->m_vecFrictionTurnForce.y = turn;

			return 0;
		}
		else { throw "bad argument in function car_rotate_turn_on_y"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int car_rotate_turn_on_z(lua_State* L) {// уст угол поворота авто по оси z.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			double speed = lua_tonumber(L, 2);
			double turn = lua_tonumber(L, 3);

			car->m_vecTurnSpeed.z = speed;
			car->m_vecFrictionTurnForce.z = turn;

			return 0;
		}
		else { throw "bad argument in function car_rotate_turn_on_z"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int setcar_resistance(lua_State* L) {// Уст силу сопротивления авто трению.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на авто.

			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			double resistance = lua_tonumber(L, 2);
			int res = int(car) + 0x0bc; //	turn resistance
			patch::SetFloat(res, resistance);

			return 0;
		}
		else { throw "bad argument in function setcar_resistance"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_car_angle(CVehicle* car, vector<float>& carang);

int getcar_turn_on_x(lua_State* L) {// Получить угол поворота авто по оси x.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);// получить указатель на авто.

			vector<float>carang{};
			carang.resize(3);
			get_car_angle(car, carang);
			float x = carang[0];

			lua_pushnumber(L, x);
			return 1;
		}
		else { throw "bad argument in function getcar_turn_on_x"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int getcar_turn_on_y(lua_State* L) {// Получить угол поворота авто по оси y.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.

			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);// получить указатель на авто.

			vector<float>carang{};
			carang.resize(3);
			get_car_angle(car, carang);

			float y = carang[1];
			lua_pushnumber(L, y);
			return 1;
		}
		else { throw "bad argument in function getcar_turn_on_y"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int getcar_turn_on_z(lua_State* L) {// Получить угол поворота авто по оси z.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.

			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);// получить указатель на авто.

			vector<float>carang{};
			carang.resize(3);
			get_car_angle(car, carang);
			float z = carang[2];

			lua_pushnumber(L, z);
			return 1;
		}
		else { throw "bad argument in function getcar_turn_on_z"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int getcar_turns_on_axes(lua_State* L){ // получить углы авто поворота по осям.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1))	{// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			float x, y, z;

			vector<float>carang{};
			carang.resize(3);
			get_car_angle(car, carang);
			x = carang[0];
			y = carang[1];
			z = carang[2];

			lua_pushnumber(L, x);
			lua_pushnumber(L, y);
			lua_pushnumber(L, z);

			return 3;
		}
		else { throw "bad argument in function get_at_on_z"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

CVector* get_car_angles_on_axes(CVehicle* car);
int turncar(CVehicle* car, float angle, float speed, int switc);
int car_turn_on_x_with_speed(lua_State* L) {//повeрнуть авто по оси x на угол со скорость.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);// получить указатель на авто.

			float angle = lua_tonumber(L, 2);
			float speed = lua_tonumber(L, 3);
			std::thread(turncar, std::ref(car), angle, speed, 1).detach();

			return 0;
		}
		else { throw "bad argument in function car_turn_on_x_with_speed"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int car_turn_on_y_with_speed(lua_State* L) {//повeрнуть авто по оси y на угол со скорость.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);// получить указатель на авто.

			float angle = lua_tonumber(L, 2);
			float speed = lua_tonumber(L, 3);
			std::thread(turncar, std::ref(car), angle, speed, 2).detach();

			return 0;
		}
		else { throw "bad argument in function car_turn_on_y_with_speed"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int car_turn_on_z_with_speed(lua_State* L) {//повeрнуть авто по оси z на угол со скорость.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);// получить указатель на авто.

			float angle = lua_tonumber(L, 2);
			float speed = lua_tonumber(L, 3);
			std::thread(turncar, std::ref(car), angle, speed, 3 ).detach();

			return 0;
		}
		else { throw "bad argument in function car_turn_on_z_with_speed"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int setcar_on_x(lua_State* L) {// уст авто по оси x.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);	float z = lua_tonumber(L, 2); //p1.RotateZ(z);


			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			CVector entpos = car->pos;

			(*(void(__thiscall*)(CMatrix*, float))0x4DF4F0)((CMatrix*)car, 3.1415927f * z * 0.0055555557f);    //    CMatrix::SetRotateZ

			car->pos = {
			car->pos.x += entpos.x,
			car->pos.y += entpos.y,
			car->pos.z += entpos.z
			};

			return 0;
		}
		else { throw "bad argument in function setcar_on_x"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int setcar_on_y(lua_State* L) {// уст авто по оси y.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);	float z = lua_tonumber(L, 2); //p1.RotateZ(z);

			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			CVector entpos = car->pos;

			(*(void(__thiscall*)(CMatrix*, float))0x4DF450)((CMatrix*)car, 3.1415927f * z * 0.0055555557f);    //    CMatrix::SetRotateZ

			car->pos = {
			car->pos.x += entpos.x,
			car->pos.y += entpos.y,
			car->pos.z += entpos.z
			};
			return 0;
		}
		else { throw "bad argument in function setcar_on_y"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int setcar_on_z(lua_State* L) {// уст авто по оси z.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);// получить указатель на авто.

			float z = lua_tonumber(L, 2); //p1.RotateZ(z);
			CVector entpos = car->pos;

			(*(void(__thiscall*)(CMatrix*, float))0x4DF3B0)((CMatrix*)car, 3.1415927f * z * 0.0055555557f);    //    CMatrix::SetRotateZ

			//car->pos = {
			car->pos.x = entpos.x;
				car->pos.y = entpos.y;
				car->pos.z = entpos.z;
			//};
			return 0;
		}
		else { throw "bad argument in function setcar_on_z"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
			/*
			    //    axis is: 1 - X, 2 - Y, 3 - Z.
    switch (axis)
    {
        case 1:
            (*(void (__thiscall*)(CMatrix*, float))0x4DF4F0)(ent->m_matrix, 3.1415927f * deg * 0.0055555557f);    //    CMatrix::SetRotateX
            break;
        case 2:
            (*(void (__thiscall*)(CMatrix*, float))0x4DF450)(ent->m_matrix, 3.1415927f * deg * 0.0055555557f);    //    CMatrix::SetRotateY
            break;
        case 3:
            (*(void (__thiscall*)(CMatrix*, float))0x4DF3B0)(ent->m_matrix, 3.1415927f * deg * 0.0055555557f);    //    CMatrix::SetRotateZ
            break;
    }
			*/
};

int turncar_with_delay(CVehicle* car, float angle, int time, int switc);
int car_turn_on_x_with_delay(lua_State* L) {//повeрнуть авто по оси x на угол со задержкой.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);// получить указатель на авто.

			float angle = lua_tonumber(L, 2);
			float time = lua_tonumber(L, 3);
			std::thread(turncar_with_delay, std::ref(car), angle, time, 1).detach();

			return 0;
		}
		else { throw "bad argument in function car_turn_on_x_with_delay"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int car_turn_on_y_with_delay(lua_State* L) {//повeрнуть авто по оси y на угол со задержкой.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);// получить указатель на авто.

			float angle = lua_tonumber(L, 2);
			float time = lua_tonumber(L, 3);
			std::thread(turncar_with_delay, std::ref(car), angle, time, 2).detach();

			return 0;
		}
		else { throw "bad argument in function car_turn_on_y_with_delay"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int car_turn_on_z_with_delay(lua_State* L) {//повeрнуть авто по оси z на угол со задержкой.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);// получить указатель на авто.

			float angle = lua_tonumber(L, 2);
			float time = lua_tonumber(L, 3);
			std::thread(turncar_with_delay, std::ref(car), angle, time, 3).detach();

			return 0;
		}
		else { throw "bad argument in function car_turn_on_z_with_delay"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int set_car_mass(lua_State* L) {// set_car_mass(car, 3000) - увеличить массу авто (значение как в handling.cfg, 3000 ~ грузовик).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);// получить указатель на авто.

			float value = lua_tonumber(L, 2);// на сколько увеличить массу авто.

			// Штатный опкод ставит флаг bIsHeavy (+2x массы при столкновениях)
			// и сам задаёт 3x/5x массы (как в миссиях). Без флага простая запись
			// m_fMass почти не влияет на поведение (масса не участвует в разгоне).
			Command<COMMAND_SET_CAR_HEAVY>(CPools::GetVehicleRef(car), true);
			car->m_fMass += value;
			car->m_fTurnMass += value;

			return 0;
		}
		else { throw "bad argument in function set_car_mass"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_car_acceleration(lua_State* L) {// set_car_acceleration(car, 0.2) - установить ускорение авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);// получить указатель на авто.

			float value = lua_tonumber(L, 2);// ускорение авто (как в handling.cfg поле идёт 14-м - engine acceleration).
			car->m_pHandlingData->m_transmissionData.m_fEngineAcceleration = value;

			return 0;
		}
		else { throw "bad argument in function set_car_acceleration"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_car_max_speed(lua_State* L) {// set_car_max_speed(car, 100) - установить максимальную скорость авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);// получить указатель на авто.

			float value = lua_tonumber(L, 2);// максимальная скорость авто в км/ч (как в handling.cfg поле 13-е - max velocity).

			// Настоящий лимитер скорости в физике - fMaxVelocity (m_fMaxGearVelocity, 0x50):
			// CalculateDriveAcceleration каждый кадр проверяет fVelocity > fMaxVelocity.
			// Поле m_fMaxSpeed (0x54) - это КРУИЗНАЯ скорость ИИ-автопилота (Vehicle.cpp:1247),
			// на скорость игрока не влияет (раньше писали в него - ничего не происходило).
			// В памяти значение в игровых единицах = км/ч / 180 (ConvertDataToGameUnits).
			float gameUnits = value / 180.0f;
			car->m_pHandlingData->m_transmissionData.m_fMaxGearVelocity = gameUnits;

			// Пересчитать передачи (Gears[]) из нового fMaxVelocity, иначе верхняя передача
			// держит старый таргет скорости и новая скорость недостижима.
			(*(void(__thiscall*)(void*))0x5B3120)(&car->m_pHandlingData->m_transmissionData);    // cTransmission::InitGearRatios

			return 0;
		}
		else { throw "bad argument in function set_car_max_speed"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_car_traction(lua_State* L) {// set_car_traction(car, 0.9) - установить сцепление авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);

			CVehicle* car = findcarinpool(p);// получить указатель на авто.

			float value = lua_tonumber(L, 2);// сцепление авто (как в handling.cfg поле идёт 9-м - traction multiplier).
			car->m_pHandlingData->fTractionMultiplier = value;

			return 0;
		}
		else { throw "bad argument in function set_car_traction"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_car_driver(lua_State* L) {// получить водителя авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			CPed* driver = car->m_pDriver;// водитель авто.
			if (driver == NULL) { return 0; }// нет водителя - ничего не отправлять.
			lua_pushlightuserdata(L, driver);// отправить указатель на педа.
			return 1;
		}
		else { throw "bad argument in function get_car_driver"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_car_passenger(lua_State* L) {// get_car_passenger(car, seat) - получить пассажира авто по номеру места (0-7).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на авто и число.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			int seat = lua_tointeger(L, 2);// номер места пассажира.
			if (seat < 0 || seat > 7) { throw "bad seat number in function get_car_passenger"; }
			CPed* passenger = car->m_passengers[seat];// пассажир авто.
			if (passenger == NULL) { return 0; }// нет пассажира - ничего не отправлять.
			lua_pushlightuserdata(L, passenger);// отправить указатель на педа.
			return 1;
		}
		else { throw "bad argument in function get_car_passenger"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_car_num_passengers(lua_State* L) {// получить кол-во пассажиров в авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			int num = car->m_nNumPassengers;// кол-во пассажиров.
			lua_pushinteger(L, num);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_car_num_passengers"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_car_max_passengers(lua_State* L) {// получить максимальное кол-во пассажиров авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			int num = car->m_nMaxPassengers;// макс кол-во пассажиров.
			lua_pushinteger(L, num);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_car_max_passengers"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_car_steer_angle(lua_State* L) {// получить угол поворота руля авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			float angle = car->m_fSteerAngle;// угол руля.
			lua_pushnumber(L, angle);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_car_steer_angle"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_car_steer_angle(lua_State* L) {// установить угол поворота руля авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на авто и число.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			float angle = lua_tonumber(L, 2);// угол руля.
			car->m_fSteerAngle = angle;// установить угол руля.
			return 0;
		}
		else { throw "bad argument in function set_car_steer_angle"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_car_gas_pedal(lua_State* L) {// получить положение педали газа авто (0.0-1.0).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			float gas = car->m_fGasPedal;// педаль газа.
			lua_pushnumber(L, gas);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_car_gas_pedal"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_car_brake_pedal(lua_State* L) {// получить положение педали тормоза авто (0.0-1.0).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			float brake = car->m_fBreakPedal;// педаль тормоза.
			lua_pushnumber(L, brake);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_car_brake_pedal"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_car_engine_on(lua_State* L) {// работает ли двигатель авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			bool on = car->bEngineOn;// двигатель работает.
			lua_pushboolean(L, on);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_car_engine_on"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_car_engine_on(lua_State* L) {// включить или выключить двигатель авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {// указатель на авто и булево.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			bool on = lua_toboolean(L, 2);// вкл/выкл двигатель.
			car->bEngineOn = on;// установить состояние двигателя.
			return 0;
		}
		else { throw "bad argument in function set_car_engine_on"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_car_lights_on(lua_State* L) {// включены ли фары авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			bool on = car->bLightsOn;// фары включены.
			lua_pushboolean(L, on);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_car_lights_on"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_car_lights_on(lua_State* L) {// включить или выключить фары авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {// указатель на авто и булево.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			bool on = lua_toboolean(L, 2);// вкл/выкл фары.
			car->bLightsOn = on;// установить состояние фар.
			return 0;
		}
		else { throw "bad argument in function set_car_lights_on"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_car_handbrake_on(lua_State* L) {// включен ли ручной тормоз авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			bool on = car->bIsHandbrakeOn;// ручник включен.
			lua_pushboolean(L, on);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_car_handbrake_on"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_car_handbrake_on(lua_State* L) {// включить или выключить ручной тормоз авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {// указатель на авто и булево.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			bool on = lua_toboolean(L, 2);// вкл/выкл ручник.
			car->bIsHandbrakeOn = on;// установить ручник.
			return 0;
		}
		else { throw "bad argument in function set_car_handbrake_on"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_car_locked(lua_State* L) {// заблокировано ли авто скриптом.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			bool locked = car->bIsLocked;// авто заблокировано.
			lua_pushboolean(L, locked);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_car_locked"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_car_locked(lua_State* L) {// заблокировать или разблокировать авто скриптом.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {// указатель на авто и булево.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			bool locked = lua_toboolean(L, 2);// заблокировать/разблокировать.
			car->bIsLocked = locked;// установить блокировку.
			return 0;
		}
		else { throw "bad argument in function set_car_locked"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_car_is_van(lua_State* L) {// авто это фургон.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			bool van = car->bIsVan;// фургон или нет.
			lua_pushboolean(L, van);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_car_is_van"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_car_is_bus(lua_State* L) {// авто это автобус.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			bool bus = car->bIsBus;// автобус или нет.
			lua_pushboolean(L, bus);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_car_is_bus"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_car_is_big(lua_State* L) {// авто это большая машина.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			bool big = car->bIsBig;// большая или нет.
			lua_pushboolean(L, big);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_car_is_big"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_car_comedy_controls(lua_State* L) {// вкл/выкл шуточное управление авто (машину крутит).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {// указатель на авто и булево.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			bool comedy = lua_toboolean(L, 2);// вкл/выкл шуточное управление.
			car->bComedyControls = comedy;// установить шуточное управление.
			return 0;
		}
		else { throw "bad argument in function set_car_comedy_controls"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_car_hide_occupants(lua_State* L) {// скрыть/показать седоков авто (как в миссиях).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {// указатель на авто и булево.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			bool hide = lua_toboolean(L, 2);// скрыть/показать седоков.
			car->bHideOccupants = hide;// установить скрытие седоков.
			return 0;
		}
		else { throw "bad argument in function set_car_hide_occupants"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_car_tires_invulnerable(lua_State* L) {// шины авто неуязвимы.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			bool inv = car->bCarTiresInvulnerable;// шины неуязвимы.
			lua_pushboolean(L, inv);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_car_tires_invulnerable"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_car_tires_invulnerable(lua_State* L) {// сделать шины авто неуязвимыми.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {// указатель на авто и булево.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			bool inv = lua_toboolean(L, 2);// вкл/выкл неуязвимость шин.
			car->bCarTiresInvulnerable = inv;// установить неуязвимость шин.
			return 0;
		}
		else { throw "bad argument in function set_car_tires_invulnerable"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_car_door_lock(lua_State* L) {// получить тип блокировки дверей авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			int lock = car->m_eDoorLock;// тип блокировки дверей.
			lua_pushinteger(L, lock);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_car_door_lock"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_car_door_lock(lua_State* L) {// установить тип блокировки дверей авто (0 - каршаринг, 1 - разблокировано, 2 - заблокировано,...).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на авто и число.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			int lock = lua_tointeger(L, 2);// тип блокировки дверей.
			car->m_eDoorLock = (eDoorLock)lock;// установить блокировку.
			return 0;
		}
		else { throw "bad argument in function set_car_door_lock"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_car_last_damager(lua_State* L) {// получить сущность которая нанесла последний урон авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			CEntity* damager = car->pLastDamEntity;// сущность нанесшая урон.
			if (damager == NULL) { return 0; }// нет сущности - ничего не отправлять.
			lua_pushlightuserdata(L, damager);// отправить указатель на сущность.
			return 1;
		}
		else { throw "bad argument in function get_car_last_damager"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_car_time_of_death(lua_State* L) {// получить время смерти авто (в игровых тиках).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			int time = car->m_nTimeOfDeath;// время смерти авто.
			lua_pushinteger(L, time);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_car_time_of_death"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_car_weapon_damage(lua_State* L) {// получить тип оружия которым нанесен последний урон авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			int weapon = car->m_nLastWeaponDamage;// тип оружия последнего урона.
			lua_pushinteger(L, weapon);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_car_weapon_damage"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_car_vehicle_class(lua_State* L) {// получить класс авто (автомобиль, велосипед, вертолет,...).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			int vclass = car->m_nVehicleClass;// класс авто.
			lua_pushinteger(L, vclass);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_car_vehicle_class"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int car_play_horn(lua_State* L) {// авто сигналит.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			car->PlayCarHorn();// посигналить.
			return 0;
		}
		else { throw "bad argument in function car_play_horn"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int car_extinguish_fire(lua_State* L) {// потушить пожар на авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			car->ExtinguishCarFire();// потушить пожар.
			return 0;
		}
		else { throw "bad argument in function car_extinguish_fire"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int car_uses_siren(lua_State* L) {// есть ли у авто сирена.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			bool siren = car->UsesSiren();// есть сирена или нет.
			lua_pushboolean(L, siren);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function car_uses_siren"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int car_is_law_enforcement(lua_State* L) {// авто принадлежит силам правопорядка.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			bool law = car->IsLawEnforcementVehicle();// полицейское авто или нет.
			lua_pushboolean(L, law);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function car_is_law_enforcement"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int car_has_roof(lua_State* L) {// у авто есть крыша.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			bool roof = car->CarHasRoof();// есть крыша или нет.
			lua_pushboolean(L, roof);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function car_has_roof"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int car_is_open_top(lua_State* L) {// авто открытая (кабриолет).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			bool open = car->IsOpenTopCar();// кабриолет или нет.
			lua_pushboolean(L, open);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function car_is_open_top"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int car_is_upside_down(lua_State* L) {// авто перевернуто вверх колесами.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			bool down = car->IsUpsideDown();// перевернуто или нет.
			lua_pushboolean(L, down);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function car_is_upside_down"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int car_is_on_its_side(lua_State* L) {// авто лежит на боку.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			bool side = car->IsOnItsSide();// на боку или нет.
			lua_pushboolean(L, side);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function car_is_on_its_side"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int car_is_vehicle_normal(lua_State* L) {// авто в нормальном состоянии (не миссионное).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			bool normal = car->IsVehicleNormal();// нормальное или нет.
			lua_pushboolean(L, normal);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function car_is_vehicle_normal"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int car_can_be_deleted(lua_State* L) {// можно ли удалить авто из мира.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			bool can = car->CanBeDeleted();// можно удалить или нет.
			lua_pushboolean(L, can);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function car_can_be_deleted"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int car_can_ped_enter(lua_State* L) {// пед может сесть в авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			bool can = car->CanPedEnterCar();// можно сесть или нет.
			lua_pushboolean(L, can);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function car_can_ped_enter"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int car_can_doors_be_damaged(lua_State* L) {// двери авто могут быть повреждены.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			bool can = car->CanDoorsBeDamaged();// могут быть повреждены или нет.
			lua_pushboolean(L, can);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function car_can_doors_be_damaged"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int car_activate_bomb(lua_State* L) {// активировать бомбу в авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			car->ActivateBomb();// активировать бомбу.
			return 0;
		}
		else { throw "bad argument in function car_activate_bomb"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int car_activate_bomb_when_entered(lua_State* L) {// активировать бомбу при посадке в авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			car->ActivateBombWhenEntered();// активировать бомбу при посадке.
			return 0;
		}
		else { throw "bad argument in function car_activate_bomb_when_entered"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int car_get_height_above_road(lua_State* L) {// получить высоту авто над дорогой.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			float height = car->GetHeightAboveRoad();// высота над дорогой.
			lua_pushnumber(L, height);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function car_get_height_above_road"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int car_is_clear_to_drive_away(lua_State* L) {// может ли авто свободно уехать (ничего не мешает).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			bool clear = car->IsClearToDriveAway();// можно уехать или нет.
			lua_pushboolean(L, clear);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function car_is_clear_to_drive_away"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int car_is_door_ready(lua_State* L) {// car_is_door_ready(car, door) - дверь авто готова открыться (0-капот,1-багажник,2-5 двери).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на авто и число.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			int door = lua_tointeger(L, 2);// номер двери.
			bool ready = car->IsDoorReady((eDoors)door);// дверь готова или нет.
			lua_pushboolean(L, ready);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function car_is_door_ready"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int car_is_door_fully_open(lua_State* L) {// car_is_door_fully_open(car, door) - дверь авто полностью открыта.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на авто и число.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			int door = lua_tointeger(L, 2);// номер двери.
			bool open = car->IsDoorFullyOpen((eDoors)door);// дверь открыта или нет.
			lua_pushboolean(L, open);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function car_is_door_fully_open"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int car_is_door_closed(lua_State* L) {// car_is_door_closed(car, door) - дверь авто закрыта.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на авто и число.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			int door = lua_tointeger(L, 2);// номер двери.
			bool closed = car->IsDoorClosed((eDoors)door);// дверь закрыта или нет.
			lua_pushboolean(L, closed);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function car_is_door_closed"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int car_is_door_missing(lua_State* L) {// car_is_door_missing(car, door) - дверь авто отсутствует (сорвана).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на авто и число.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			int door = lua_tointeger(L, 2);// номер двери.
			bool missing = car->IsDoorMissing((eDoors)door);// дверь отсутствует или нет.
			lua_pushboolean(L, missing);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function car_is_door_missing"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int car_shuffle_passengers(lua_State* L) {// пересадить пассажиров авто чтобы освободить место.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			bool shuffled = car->ShufflePassengersToMakeSpace();// пересадить пассажиров.
			lua_pushboolean(L, shuffled);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function car_shuffle_passengers"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int car_update_passenger_list(lua_State* L) {// обновить список пассажиров авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			car->UpdatePassengerList();// обновить список пассажиров.
			return 0;
		}
		else { throw "bad argument in function car_update_passenger_list"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_car_radio_station(lua_State* L) {// получить номер радиостанции авто (старая версия).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			int radio = car->m_nRadioStation;// номер радиостанции.
			lua_pushinteger(L, radio);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_car_radio_station"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_car_radio_station(lua_State* L) {// set_car_radio_station(car, station) - установить радиостанцию авто (0-9).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на авто и число.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			int station = lua_tointeger(L, 2);// номер радиостанции.
			car->m_nRadioStation = station;// установить радиостанцию.
			return 0;
		}
		else { throw "bad argument in function set_car_radio_station"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_car_created_by(lua_State* L) {// получить кем создано авто (0-игра,1-скрипт,2-пикап).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			int by = car->m_nCreatedBy;// кем создано.
			lua_pushinteger(L, by);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_car_created_by"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_car_wanted_stars_on_enter(lua_State* L) {// получить кол-во звёзд розыска при входе в авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на авто.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			int stars = car->m_wWantedStarsOnEnter;// звёзды розыска.
			lua_pushinteger(L, stars);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_car_wanted_stars_on_enter"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_car_wanted_stars_on_enter(lua_State* L) {// set_car_wanted_stars_on_enter(car, stars) - установить звёзды розыска при входе в авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на авто и число.
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			int stars = lua_tointeger(L, 2);// звёзды розыска.
			car->m_wWantedStarsOnEnter = stars;// установить звёзды.
			return 0;
		}
		else { throw "bad argument in function set_car_wanted_stars_on_enter"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_weapon_range(lua_State* L) {// получить дальность стрельбы текущего оружия педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			CWeapon weapon = ped->m_aWeapons[ped->m_nCurrentWeapon];
			eWeaponType type = weapon.m_eWeaponType;
			CWeaponInfo* w = CWeaponInfo::GetWeaponInfo(type);
			float range = w->m_fRange;// дальность оружия.
			lua_pushnumber(L, range);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_weapon_range"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_weapon_range(lua_State* L) {// set_weapon_range(ped, 50.0) - установить дальность стрельбы текущего оружия педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на педа и число.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			float range = lua_tonumber(L, 2);// дальность оружия.
			CWeapon weapon = ped->m_aWeapons[ped->m_nCurrentWeapon];
			eWeaponType type = weapon.m_eWeaponType;
			CWeaponInfo* w = CWeaponInfo::GetWeaponInfo(type);
			w->m_fRange = range;// установить дальность.
			return 0;
		}
		else { throw "bad argument in function set_weapon_range"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_weapon_firing_rate(lua_State* L) {// получить скорость стрельбы текущего оружия педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			CWeapon weapon = ped->m_aWeapons[ped->m_nCurrentWeapon];
			eWeaponType type = weapon.m_eWeaponType;
			CWeaponInfo* w = CWeaponInfo::GetWeaponInfo(type);
			int rate = w->m_nFiringRate;// скорость стрельбы.
			lua_pushinteger(L, rate);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_weapon_firing_rate"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_weapon_firing_rate(lua_State* L) {// set_weapon_firing_rate(ped, 250) - установить скорость стрельбы текущего оружия педа (меньше - быстрее).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на педа и число.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			int rate = lua_tointeger(L, 2);// скорость стрельбы.
			CWeapon weapon = ped->m_aWeapons[ped->m_nCurrentWeapon];
			eWeaponType type = weapon.m_eWeaponType;
			CWeaponInfo* w = CWeaponInfo::GetWeaponInfo(type);
			w->m_nFiringRate = rate;// установить скорость стрельбы.
			return 0;
		}
		else { throw "bad argument in function set_weapon_firing_rate"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_weapon_reload(lua_State* L) {// получить время перезарядки текущего оружия педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			CWeapon weapon = ped->m_aWeapons[ped->m_nCurrentWeapon];
			eWeaponType type = weapon.m_eWeaponType;
			CWeaponInfo* w = CWeaponInfo::GetWeaponInfo(type);
			int reload = w->m_nReload;// время перезарядки.
			lua_pushinteger(L, reload);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_weapon_reload"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_weapon_reload(lua_State* L) {// set_weapon_reload(ped, 450) - установить время перезарядки текущего оружия педа (меньше - быстрее).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на педа и число.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			int reload = lua_tointeger(L, 2);// время перезарядки.
			CWeapon weapon = ped->m_aWeapons[ped->m_nCurrentWeapon];
			eWeaponType type = weapon.m_eWeaponType;
			CWeaponInfo* w = CWeaponInfo::GetWeaponInfo(type);
			w->m_nReload = reload;// установить время перезарядки.
			return 0;
		}
		else { throw "bad argument in function set_weapon_reload"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_weapon_speed(lua_State* L) {// получить скорость пули текущего оружия педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			CWeapon weapon = ped->m_aWeapons[ped->m_nCurrentWeapon];
			eWeaponType type = weapon.m_eWeaponType;
			CWeaponInfo* w = CWeaponInfo::GetWeaponInfo(type);
			float speed = w->m_fSpeed;// скорость пули.
			lua_pushnumber(L, speed);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_weapon_speed"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_weapon_speed(lua_State* L) {// set_weapon_speed(ped, 100.0) - установить скорость пули текущего оружия педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на педа и число.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			float speed = lua_tonumber(L, 2);// скорость пули.
			CWeapon weapon = ped->m_aWeapons[ped->m_nCurrentWeapon];
			eWeaponType type = weapon.m_eWeaponType;
			CWeaponInfo* w = CWeaponInfo::GetWeaponInfo(type);
			w->m_fSpeed = speed;// установить скорость пули.
			return 0;
		}
		else { throw "bad argument in function set_weapon_speed"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_weapon_spread(lua_State* L) {// получить разброс текущего оружия педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			CWeapon weapon = ped->m_aWeapons[ped->m_nCurrentWeapon];
			eWeaponType type = weapon.m_eWeaponType;
			CWeaponInfo* w = CWeaponInfo::GetWeaponInfo(type);
			float spread = w->m_fSpread;// разброс оружия.
			lua_pushnumber(L, spread);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_weapon_spread"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_weapon_spread(lua_State* L) {// set_weapon_spread(ped, 0.5) - установить разброс текущего оружия педа (больше - сильнее разброс).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на педа и число.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			float spread = lua_tonumber(L, 2);// разброс оружия.
			CWeapon weapon = ped->m_aWeapons[ped->m_nCurrentWeapon];
			eWeaponType type = weapon.m_eWeaponType;
			CWeaponInfo* w = CWeaponInfo::GetWeaponInfo(type);
			w->m_fSpread = spread;// установить разброс.
			return 0;
		}
		else { throw "bad argument in function set_weapon_spread"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_weapon_radius(lua_State* L) {// получить радиус поражения текущего оружия педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			CWeapon weapon = ped->m_aWeapons[ped->m_nCurrentWeapon];
			eWeaponType type = weapon.m_eWeaponType;
			CWeaponInfo* w = CWeaponInfo::GetWeaponInfo(type);
			float radius = w->m_fRadius;// радиус оружия.
			lua_pushnumber(L, radius);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_weapon_radius"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_weapon_radius(lua_State* L) {// set_weapon_radius(ped, 5.0) - установить радиус поражения текущего оружия педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на педа и число.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			float radius = lua_tonumber(L, 2);// радиус оружия.
			CWeapon weapon = ped->m_aWeapons[ped->m_nCurrentWeapon];
			eWeaponType type = weapon.m_eWeaponType;
			CWeaponInfo* w = CWeaponInfo::GetWeaponInfo(type);
			w->m_fRadius = radius;// установить радиус.
			return 0;
		}
		else { throw "bad argument in function set_weapon_radius"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_weapon_lifespan(lua_State* L) {// получить время жизни снаряда текущего оружия педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			CWeapon weapon = ped->m_aWeapons[ped->m_nCurrentWeapon];
			eWeaponType type = weapon.m_eWeaponType;
			CWeaponInfo* w = CWeaponInfo::GetWeaponInfo(type);
			float lifespan = w->m_fLifespan;// время жизни снаряда.
			lua_pushnumber(L, lifespan);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_weapon_lifespan"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_weapon_lifespan(lua_State* L) {// set_weapon_lifespan(ped, 100.0) - установить время жизни снаряда текущего оружия педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на педа и число.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			float lifespan = lua_tonumber(L, 2);// время жизни снаряда.
			CWeapon weapon = ped->m_aWeapons[ped->m_nCurrentWeapon];
			eWeaponType type = weapon.m_eWeaponType;
			CWeaponInfo* w = CWeaponInfo::GetWeaponInfo(type);
			w->m_fLifespan = lifespan;// установить время жизни.
			return 0;
		}
		else { throw "bad argument in function set_weapon_lifespan"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_weapon_slot_info(lua_State* L) {// получить слот текущего оружия педа (номер группы оружия).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			CWeapon weapon = ped->m_aWeapons[ped->m_nCurrentWeapon];
			eWeaponType type = weapon.m_eWeaponType;
			CWeaponInfo* w = CWeaponInfo::GetWeaponInfo(type);
			int slot = w->m_WeaponSlot;// слот оружия.
			lua_pushinteger(L, slot);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_weapon_slot_info"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_ammo_in_clip_ped(lua_State* L) {// получить кол-во патронов в обойме текущего оружия педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			int ammo = ped->m_aWeapons[ped->m_nCurrentWeapon].m_nAmmoInClip;// патроны в обойме.
			lua_pushinteger(L, ammo);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_ammo_in_clip_ped"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_ammo_in_clip_ped(lua_State* L) {// set_ammo_in_clip_ped(ped, 30) - установить кол-во патронов в обойме текущего оружия педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на педа и число.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			int ammo = lua_tointeger(L, 2);// патроны в обойме.
			ped->m_aWeapons[ped->m_nCurrentWeapon].m_nAmmoInClip = ammo;// установить патроны.
			return 0;
		}
		else { throw "bad argument in function set_ammo_in_clip_ped"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_weapon_state(lua_State* L) {// получить состояние текущего оружия педа (0-готово,1-стреляет,2-перезарядка,3-нет патронов).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			int state = ped->m_aWeapons[ped->m_nCurrentWeapon].m_eWeaponState;// состояние оружия.
			lua_pushinteger(L, state);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_weapon_state"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int weapon_reload_ped(lua_State* L) {// перезарядить текущее оружие педа.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			ped->m_aWeapons[ped->m_nCurrentWeapon].Reload();// перезарядить оружие.
			return 0;
		}
		else { throw "bad argument in function weapon_reload_ped"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int weapon_is_melee(lua_State* L) {// текущее оружие педа рукопашное.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			bool melee = ped->m_aWeapons[ped->m_nCurrentWeapon].IsTypeMelee();// рукопашное или нет.
			lua_pushboolean(L, melee);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function weapon_is_melee"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int weapon_is_two_handed(lua_State* L) {// текущее оружие педа двуручное.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			bool handed = ped->m_aWeapons[ped->m_nCurrentWeapon].IsType2Handed();// двуручное или нет.
			lua_pushboolean(L, handed);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function weapon_is_two_handed"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int weapon_has_ammo_to_be_used(lua_State* L) {// текущее оружие педа можно использовать (есть патроны).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			bool ammo = ped->m_aWeapons[ped->m_nCurrentWeapon].HasWeaponAmmoToBeUsed();// есть патроны или нет.
			lua_pushboolean(L, ammo);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function weapon_has_ammo_to_be_used"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_player_money_info(lua_State* L) {// получить кол-во денег игрока напрямую.
	try {
		CPlayerPed& player = *FindPlayerPed();
		CPlayerInfo* info = player.GetPlayerInfoForThisPlayerPed();
		if (info == NULL) { return 0; }// нет инфы - ничего не отправлять.
		int money = info->m_nMoney;// деньги игрока.
		lua_pushinteger(L, money);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_player_money_info(lua_State* L) {// set_player_money_info(money) - установить кол-во денег игроку напрямую.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// число.
			int money = lua_tointeger(L, 1);// деньги игрока.
			CPlayerPed& player = *FindPlayerPed();
			CPlayerInfo* info = player.GetPlayerInfoForThisPlayerPed();
			if (info == NULL) { return 0; }// нет инфы - выйти.
			info->m_nMoney = money;// установить деньги.
			return 0;
		}
		else { throw "bad argument in function set_player_money_info"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_player_display_money(lua_State* L) {// получить отображаемые деньги игрока.
	try {
		CPlayerPed& player = *FindPlayerPed();
		CPlayerInfo* info = player.GetPlayerInfoForThisPlayerPed();
		if (info == NULL) { return 0; }// нет инфы - ничего не отправлять.
		int money = info->m_nDisplayMoney;// отображаемые деньги.
		lua_pushinteger(L, money);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_player_drive_by(lua_State* L) {// set_player_drive_by(true/false) - разрешить или запретить игроку стрельбу на ходу из авто.
	try {
		if (LUA_TBOOLEAN == lua_type(L, 1)) {// булево.
			bool can = lua_toboolean(L, 1);// разрешить/запретить.
			CPlayerPed& player = *FindPlayerPed();
			CPlayerInfo* info = player.GetPlayerInfoForThisPlayerPed();
			if (info == NULL) { return 0; }// нет инфы - выйти.
			info->m_bCanDoDriveBy = can;// установить стрельбу на ходу.
			return 0;
		}
		else { throw "bad argument in function set_player_drive_by"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_wanted_chaos_level(lua_State* L) {// получить уровень хаоса (очки розыска) игрока.
	try {
		CPlayerPed& player = *FindPlayerPed();
		int chaos = player.GetWanted()->m_nChaosLevel;// очки хаоса.
		lua_pushinteger(L, chaos);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_wanted_chaos_level(lua_State* L) {// set_wanted_chaos_level(points) - установить очки хаоса розыска игрока.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// число.
			int chaos = lua_tointeger(L, 1);// очки хаоса.
			CPlayerPed& player = *FindPlayerPed();
			CPlayerInfo* info = player.GetPlayerInfoForThisPlayerPed();
			if (info == NULL) { return 0; }// нет инфы - выйти.
			player.GetWanted()->m_nChaosLevel = chaos;// установить очки.
			return 0;
		}
		else { throw "bad argument in function set_wanted_chaos_level"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_wanted_multiplier(lua_State* L) {// получить множитель розыска игрока.
	try {
		CPlayerPed& player = *FindPlayerPed();
		float mult = player.GetWanted()->m_fMultiplier;// множитель розыска.
		lua_pushnumber(L, mult);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_wanted_multiplier(lua_State* L) {// set_wanted_multiplier(1.0) - установить множитель розыска игрока.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// число.
			float mult = lua_tonumber(L, 1);// множитель розыска.
			CPlayerPed& player = *FindPlayerPed();
			CPlayerInfo* info = player.GetPlayerInfoForThisPlayerPed();
			if (info == NULL) { return 0; }// нет инфы - выйти.
			player.GetWanted()->m_fMultiplier = mult;// установить множifier.
			return 0;
		}
		else { throw "bad argument in function set_wanted_multiplier"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_cops_in_pursuit(lua_State* L) {// получить кол-во копов преследующих игрока.
	try {
		CPlayerPed& player = *FindPlayerPed();
		int cops = player.GetWanted()->m_nCopsInPursuit;// копы в погоне.
		lua_pushinteger(L, cops);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_max_cops_in_pursuit(lua_State* L) {// получить макс кол-во копов в погоне за игроком.
	try {
		CPlayerPed& player = *FindPlayerPed();
		int cops = player.GetWanted()->m_nMaxCopsInPursuit;// макс копы.
		lua_pushinteger(L, cops);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_max_cops_in_pursuit(lua_State* L) {// set_max_cops_in_pursuit(10) - установить макс кол-во копов в погоне.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// число.
			int cops = lua_tointeger(L, 1);// макс копы.
			CPlayerPed& player = *FindPlayerPed();
			CPlayerInfo* info = player.GetPlayerInfoForThisPlayerPed();
			if (info == NULL) { return 0; }// нет инфы - выйти.
			player.GetWanted()->m_nMaxCopsInPursuit = cops;// установить макс копов.
			return 0;
		}
		else { throw "bad argument in function set_max_cops_in_pursuit"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_max_cop_cars_in_pursuit(lua_State* L) {// получить макс кол-во полицейских авто в погоне.
	try {
		CPlayerPed& player = *FindPlayerPed();
		int cars = player.GetWanted()->m_nMaxCopCarsInPursuit;// макс полицейских авто.
		lua_pushinteger(L, cars);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_max_cop_cars_in_pursuit(lua_State* L) {// set_max_cop_cars_in_pursuit(5) - установить макс кол-во полицейских авто в погоне.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// число.
			int cars = lua_tointeger(L, 1);// макс полицейских авто.
			CPlayerPed& player = *FindPlayerPed();
			CPlayerInfo* info = player.GetPlayerInfoForThisPlayerPed();
			if (info == NULL) { return 0; }// нет инфы - выйти.
			player.GetWanted()->m_nMaxCopCarsInPursuit = cars;// установить макс авто.
			return 0;
		}
		else { throw "bad argument in function set_max_cop_cars_in_pursuit"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_chance_on_roadblock(lua_State* L) {// получить шанс появления блокпоста (0-255).
	try {
		CPlayerPed& player = *FindPlayerPed();
		int chance = player.GetWanted()->m_nChanceOnRoadBlock;// шанс блокпоста.
		lua_pushinteger(L, chance);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_chance_on_roadblock(lua_State* L) {// set_chance_on_roadblock(200) - установить шанс появления блокпоста (0-255).
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// число.
			int chance = lua_tointeger(L, 1);// шанс блокпоста.
			CPlayerPed& player = *FindPlayerPed();
			CPlayerInfo* info = player.GetPlayerInfoForThisPlayerPed();
			if (info == NULL) { return 0; }// нет инфы - выйти.
			player.GetWanted()->m_nChanceOnRoadBlock = chance;// установить шанс.
			return 0;
		}
		else { throw "bad argument in function set_chance_on_roadblock"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int wanted_are_swat_required(lua_State* L) {// нужны ли SWAT игроку по розыску.
	try {
		CPlayerPed& player = *FindPlayerPed();
		bool swat = player.GetWanted()->AreSwatRequired();// нужен SWAT.
		lua_pushboolean(L, swat);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int wanted_are_fbi_required(lua_State* L) {// нужны ли FBI игроку по розыску.
	try {
		CPlayerPed& player = *FindPlayerPed();
		bool fbi = player.GetWanted()->AreFbiRequired();// нужен FBI.
		lua_pushboolean(L, fbi);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int wanted_are_army_required(lua_State* L) {// нужна ли армия игроку по розыску.
	try {
		CPlayerPed& player = *FindPlayerPed();
		bool army = player.GetWanted()->AreArmyRequired();// нужна армия.
		lua_pushboolean(L, army);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int wanted_num_helis_required(lua_State* L) {// нужно ли вертолетов игроку по розыску.
	try {
		CPlayerPed& player = *FindPlayerPed();
		bool helis = player.GetWanted()->NumOfHelisRequired();// нужны вертолеты.
		lua_pushboolean(L, helis);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int wanted_clear_crimes(lua_State* L) {// очистить очередь преступлений игрока.
	try {
		CPlayerPed& player = *FindPlayerPed();
		CPlayerInfo* info = player.GetPlayerInfoForThisPlayerPed();
		if (info == NULL) { return 0; }// нет инфы - выйти.
		player.GetWanted()->ClearQdCrimes();// очистить очередь.
		return 0;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int wanted_reset_police_pursuit(lua_State* L) {// сбросить полицейскую погоню за игроком.
	try {
		CPlayerPed& player = *FindPlayerPed();
		CPlayerInfo* info = player.GetPlayerInfoForThisPlayerPed();
		if (info == NULL) { return 0; }// нет инфы - выйти.
		player.GetWanted()->ResetPolicePursuit();// сбросить погоню.
		return 0;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int wanted_update_wanted_level(lua_State* L) {// обновить уровень розыска игрока (пересчитать звёзды).
	try {
		CPlayerPed& player = *FindPlayerPed();
		CPlayerInfo* info = player.GetPlayerInfoForThisPlayerPed();
		if (info == NULL) { return 0; }// нет инфы - выйти.
		player.GetWanted()->UpdateWantedLevel();// обновить уровень.
		return 0;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int wanted_set_no_drop(lua_State* L) {// wanted_set_no_drop(level) - установить уровень розыска который не может опуститься.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// число.
			int level = lua_tointeger(L, 1);// уровень розыска.
			CPlayerPed& player = *FindPlayerPed();
			CPlayerInfo* info = player.GetPlayerInfoForThisPlayerPed();
			if (info == NULL) { return 0; }// нет инфы - выйти.
			player.GetWanted()->SetWantedLevelNoDrop(level);// установить без понижения.
			return 0;
		}
		else { throw "bad argument in function wanted_set_no_drop"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int wanted_cheat_level(lua_State* L) {// wanted_cheat_level(level) - читерски установить уровень розыска.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// число.
			int level = lua_tointeger(L, 1);// уровень розыска.
			CPlayerPed& player = *FindPlayerPed();
			CPlayerInfo* info = player.GetPlayerInfoForThisPlayerPed();
			if (info == NULL) { return 0; }// нет инфы - выйти.
			player.GetWanted()->CheatWantedLevel(level);// читерская установка.
			return 0;
		}
		else { throw "bad argument in function wanted_cheat_level"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_wanted_level_before_parole(lua_State* L) {// получить уровень розыска до условно-досрочного.
	try {
		CPlayerPed& player = *FindPlayerPed();
		int level = player.GetWanted()->m_nWantedLevelBeforeParole;// уровень до пароля.
		lua_pushinteger(L, level);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_wanted_last_decreased_time(lua_State* L) {// получить время последнего понижения розыска.
	try {
		CPlayerPed& player = *FindPlayerPed();
		int time = player.GetWanted()->m_nLastTimeWantedDecreased;// время понижения.
		lua_pushinteger(L, time);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int force_weather(lua_State* L) {// force_weather(type) - плавно сменить погоду (0-ясно,...,11-гроза).
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// число.
			int type = lua_tointeger(L, 1);// тип погоды.
			CWeather::ForceWeather(type);// сменить погоду плавно.
			return 0;
		}
		else { throw "bad argument in function force_weather"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int force_weather_now(lua_State* L) {// force_weather_now(type) - сменить погоду мгновенно.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// число.
			int type = lua_tointeger(L, 1);// тип погоды.
			CWeather::ForceWeatherNow(type);// сменить погоду сразу.
			return 0;
		}
		else { throw "bad argument in function force_weather_now"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int release_weather(lua_State* L) {// отпустить погоду (вернуть к обычной игре).
	try {
		CWeather::ReleaseWeather();// отпустить погоду.
		return 0;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_old_weather_type(lua_State* L) {// получить прошлый тип погоды.
	try {
		int type = CWeather::OldWeatherType;// прошлый тип погоды.
		lua_pushinteger(L, type);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_new_weather_type(lua_State* L) {// получить текущий тип погоды.
	try {
		int type = CWeather::NewWeatherType;// текущий тип погоды.
		lua_pushinteger(L, type);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_weather_type(lua_State* L) {// set_weather_type(type) - установить тип погоды напрямую (без форсажа).
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// число.
			int type = lua_tointeger(L, 1);// тип погоды.
			CWeather::NewWeatherType = type;// установить тип погоды.
			return 0;
		}
		else { throw "bad argument in function set_weather_type"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int is_lightning_flash(lua_State* L) {// идёт ли вспышка молнии.
	try {
		bool flash = CWeather::LightningFlash;// вспышка молнии.
		lua_pushboolean(L, flash);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int force_rain(lua_State* L) {// force_rain(value) - установить силу дождя напрямую (0.0-2.0).
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// число.
			float value = lua_tonumber(L, 1);// сила дождя.
			Command<COMMAND_FORCE_RAIN>(value);// установить дождь.
			return 0;
		}
		else { throw "bad argument in function force_rain"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_game_hours(lua_State* L) {// получить игровые часы.
	try {
		int hours = CClock::ms_nGameClockHours;// игровые часы.
		lua_pushinteger(L, hours);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_game_minutes(lua_State* L) {// получить игровые минуты.
	try {
		int minutes = CClock::ms_nGameClockMinutes;// игровые минуты.
		lua_pushinteger(L, minutes);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_game_seconds(lua_State* L) {// получить игровые секунды.
	try {
		int seconds = CClock::ms_nGameClockSeconds;// игровые секунды.
		lua_pushinteger(L, seconds);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_game_hours(lua_State* L) {// set_game_hours(hours) - установить игровые часы (0-23).
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// число.
			int hours = lua_tointeger(L, 1);// часы.
			CClock::SetGameClock(hours, CClock::ms_nGameClockMinutes);// установить часы.
			return 0;
		}
		else { throw "bad argument in function set_game_hours"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_milliseconds_per_game_minute(lua_State* L) {// set_milliseconds_per_game_minute(ms) - установить скорость хода времени (мс на игровую минуту, меньше - быстрее).
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// число.
			int ms = lua_tointeger(L, 1);// мс на минуту.
			CClock::ms_nMillisecondsPerGameMinute = ms;// установить скорость времени.
			return 0;
		}
		else { throw "bad argument in function set_milliseconds_per_game_minute"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_milliseconds_per_game_minute(lua_State* L) {// получить скорость хода времени (мс на игровую минуту).
	try {
		int ms = CClock::ms_nMillisecondsPerGameMinute;// мс на минуту.
		lua_pushinteger(L, ms);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int clock_store(lua_State* L) {// запомнить игровое время (для восстановления позже).
	try {
		CClock::StoreClock();// запомнить время.
		return 0;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int clock_restore(lua_State* L) {// восстановить запомненное игровое время.
	try {
		CClock::RestoreClock();// восстановить время.
		return 0;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_time_until(lua_State* L) {// get_time_until(hours, minutes) - получить кол-во минут до указанного времени.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// два числа.
			int hours = lua_tointeger(L, 1);// часы.
			int minutes = lua_tointeger(L, 2);// минуты.
			int until = CClock::GetGameClockMinutesUntil(hours, minutes);// минуты до времени.
			lua_pushinteger(L, until);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_time_until"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int is_time_in_range(lua_State* L) {// is_time_in_range(hourA, hourB) - текущее время между двумя часами.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// два числа.
			int a = lua_tointeger(L, 1);// час A.
			int b = lua_tointeger(L, 2);// час B.
			bool in = CClock::GetIsTimeInRange(a, b);// время в диапазоне.
			lua_pushboolean(L, in);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function is_time_in_range"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_fov(lua_State* L) {// получить угол обзора камеры (FOV).
	try {
		float fov = CDraw::ms_fFOV;// угол обзора.
		lua_pushnumber(L, fov);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_fov(lua_State* L) {// set_fov(degrees) - установить угол обзора камеры (FOV).
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// число.
			float fov = lua_tonumber(L, 1);// угол обзора.
			CDraw::SetFOV(fov);// установить FOV.
			return 0;
		}
		else { throw "bad argument in function set_fov"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_far_clip(lua_State* L) {// получить дальность прорисовки.
	try {
		float clip = CDraw::ms_fFarClipZ;// дальность прорисовки.
		lua_pushnumber(L, clip);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_far_clip(lua_State* L) {// set_far_clip(distance) - установить дальность прорисовки.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// число.
			float clip = lua_tonumber(L, 1);// дальность прорисовки.
			CDraw::ms_fFarClipZ = clip;// установить дальность.
			return 0;
		}
		else { throw "bad argument in function set_far_clip"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_near_clip(lua_State* L) {// получить ближнюю границу прорисовки.
	try {
		float clip = CDraw::ms_fNearClipZ;// ближняя граница.
		lua_pushnumber(L, clip);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_near_clip(lua_State* L) {// set_near_clip(distance) - установить ближнюю границу прорисовки.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// число.
			float clip = lua_tonumber(L, 1);// ближняя граница.
			CDraw::ms_fNearClipZ = clip;// установить границу.
			return 0;
		}
		else { throw "bad argument in function set_near_clip"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_lod_distance(lua_State* L) {// получить дистанцию LOD моделей.
	try {
		float dist = CDraw::ms_fLODDistance;// дистанция LOD.
		lua_pushnumber(L, dist);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_lod_distance(lua_State* L) {// set_lod_distance(distance) - установить дистанцию LOD моделей.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// число.
			float dist = lua_tonumber(L, 1);// дистанция LOD.
			CDraw::ms_fLODDistance = dist;// установить дистанцию.
			return 0;
		}
		else { throw "bad argument in function set_lod_distance"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_fade_color(lua_State* L) {// получить цвет затемнения экрана (r,g,b).
	try {
		lua_pushinteger(L, CDraw::FadeRed);// красный.
		lua_pushinteger(L, CDraw::FadeGreen);// зелёный.
		lua_pushinteger(L, CDraw::FadeBlue);// синий.
		return 3;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_fade_color(lua_State* L) {// set_fade_color(r, g, b) - установить цвет затемнения экрана (0-255).
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {// три числа.
			int r = lua_tointeger(L, 1);// красный.
			int g = lua_tointeger(L, 2);// зелёный.
			int b = lua_tointeger(L, 3);// синий.
			CDraw::FadeRed = r; CDraw::FadeGreen = g; CDraw::FadeBlue = b;// установить цвет.
			return 0;
		}
		else { throw "bad argument in function set_fade_color"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int camera_shake(lua_State* L) {// camera_shake(strength, x, y, z) - тряхнуть камеру от точки.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {// четыре числа.
			float strength = lua_tonumber(L, 1);// сила тряски.
			float x = lua_tonumber(L, 2); float y = lua_tonumber(L, 3); float z = lua_tonumber(L, 4);// координаты.
			TheCamera.CamShake(strength, x, y, z);// тряхнуть камеру.
			return 0;
		}
		else { throw "bad argument in function camera_shake"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int camera_draw_borders_widescreen(lua_State* L) {// нарисовать границы широкоэкранного режима.
	try {
		TheCamera.DrawBordersForWideScreen();// нарисовать границы.
		return 0;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int camera_clear_player_weapon_mode(lua_State* L) {// сбросить режим камеры от оружия игрока.
	try {
		TheCamera.ClearPlayerWeaponMode();// сбросить режим.
		return 0;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int camera_calculate_derived_values(lua_State* L) {// пересчитать производные значения камеры.
	try {
		TheCamera.CalculateDerivedValues();// пересчитать.
		return 0;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int camera_dont_process_obbe_cinema(lua_State* L) {// отключить обработку кино-камеры Оббе.
	try {
		TheCamera.DontProcessObbeCinemaCamera();// отключить.
		return 0;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_mouse_accel_vertical(lua_State* L) {// получить вертикальное ускорение мыши.
	try {
		float accel = CCamera::m_fMouseAccelVertical;// ускорение мыши.
		lua_pushnumber(L, accel);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_mouse_accel_vertical(lua_State* L) {// set_mouse_accel_vertical(value) - установить вертикальное ускорение мыши.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// число.
			float accel = lua_tonumber(L, 1);// ускорение мыши.
			CCamera::m_fMouseAccelVertical = accel;// установить ускорение.
			return 0;
		}
		else { throw "bad argument in function set_mouse_accel_vertical"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_mouse_accel_horizontal(lua_State* L) {// получить горизонтальное ускорение мыши.
	try {
		float accel = CCamera::m_fMouseAccelHorzntal;// ускорение мыши.
		lua_pushnumber(L, accel);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_mouse_accel_horizontal(lua_State* L) {// set_mouse_accel_horizontal(value) - установить горизонтальное ускорение мыши.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// число.
			float accel = lua_tonumber(L, 1);// ускорение мыши.
			CCamera::m_fMouseAccelHorzntal = accel;// установить ускорение.
			return 0;
		}
		else { throw "bad argument in function set_mouse_accel_horizontal"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_use_mouse_3rd_person(lua_State* L) {// используется ли мышь в третьем лице.
	try {
		bool use = CCamera::m_bUseMouse3rdPerson;// мышь в третьем лице.
		lua_pushboolean(L, use);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_sun_blocked_by_clouds(lua_State* L) {// солнце закрыто облаками.
	try {
		bool blocked = CCoronas::SunBlockedByClouds;// солнце закрыто.
		lua_pushboolean(L, blocked);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int weather_add_rain(lua_State* L) {// добавить эффект дождя (внутренний вызов).
	try {
		CWeather::AddRain();// добавить дождь.
		return 0;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int weather_add_stream_after_rain(lua_State* L) {// добавить эффект пара после дождя.
	try {
		CWeather::AddStreamAfterRain();// добавить пар.
		return 0;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int weather_render_rain_streaks(lua_State* L) {// отрисовать полосы дождя.
	try {
		CWeather::RenderRainStreaks();// отрисовать полосы.
		return 0;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_obj_type(lua_State* L) {// получить тип объекта (0-обычный,1-игрок,2-миссионный,3-временный,4-катсцена,5-шипы).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на объект.
			const void* p = lua_topointer(L, 1);
			CObject* obj = findobjinpool(p);// получить указатель на объект.
			int type = obj->m_nObjectType;// тип объекта.
			lua_pushinteger(L, type);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_obj_type"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_obj_bonus_value(lua_State* L) {// получить бонусную стоимость объекта (сколько денег за разрушение).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на объект.
			const void* p = lua_topointer(L, 1);
			CObject* obj = findobjinpool(p);// получить указатель на объект.
			int bonus = obj->m_nBonusValue;// бонус объекта.
			lua_pushinteger(L, bonus);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_obj_bonus_value"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_obj_bonus_value(lua_State* L) {// set_obj_bonus_value(obj, money) - установить бонус за разрушение объекта.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на объект и число.
			const void* p = lua_topointer(L, 1);
			CObject* obj = findobjinpool(p);// получить указатель на объект.
			int bonus = lua_tointeger(L, 2);// бонус объекта.
			obj->m_nBonusValue = bonus;// установить бонус.
			return 0;
		}
		else { throw "bad argument in function set_obj_bonus_value"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_obj_cost_value(lua_State* L) {// получить стоимость объекта (для магазинов).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на объект.
			const void* p = lua_topointer(L, 1);
			CObject* obj = findobjinpool(p);// получить указатель на объект.
			int cost = obj->m_wCostValue;// стоимость объекта.
			lua_pushinteger(L, cost);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_obj_cost_value"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_obj_cost_value(lua_State* L) {// set_obj_cost_value(obj, cost) - установить стоимость объекта.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на объект и число.
			const void* p = lua_topointer(L, 1);
			CObject* obj = findobjinpool(p);// получить указатель на объект.
			int cost = lua_tointeger(L, 2);// стоимость объекта.
			obj->m_wCostValue = cost;// установить стоимость.
			return 0;
		}
		else { throw "bad argument in function set_obj_cost_value"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_obj_damage_multiplier(lua_State* L) {// получить множитель урона объекта.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на объект.
			const void* p = lua_topointer(L, 1);
			CObject* obj = findobjinpool(p);// получить указатель на объект.
			float mult = obj->m_fDamageMultiplier;// множитель урона.
			lua_pushnumber(L, mult);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_obj_damage_multiplier"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_obj_damage_multiplier(lua_State* L) {// set_obj_damage_multiplier(obj, 5.0) - установить множитель урона объекта (больше - легче сломать).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на объект и число.
			const void* p = lua_topointer(L, 1);
			CObject* obj = findobjinpool(p);// получить указатель на объект.
			float mult = lua_tonumber(L, 2);// множитель урона.
			obj->m_fDamageMultiplier = mult;// установить множитель.
			return 0;
		}
		else { throw "bad argument in function set_obj_damage_multiplier"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_obj_collision_damage_type(lua_State* L) {// получить тип урона от столкновений объекта.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на объект.
			const void* p = lua_topointer(L, 1);
			CObject* obj = findobjinpool(p);// получить указатель на объект.
			int type = obj->m_CollisionDamageType;// тип урона.
			lua_pushinteger(L, type);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_obj_collision_damage_type"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_obj_collision_damage_type(lua_State* L) {// set_obj_collision_damage_type(obj, type) - установить тип урона от столкновений объекта.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на объект и число.
			const void* p = lua_topointer(L, 1);
			CObject* obj = findobjinpool(p);// получить указатель на объект.
			int type = lua_tointeger(L, 2);// тип урона.
			obj->m_CollisionDamageType = type;// установить тип урона.
			return 0;
		}
		else { throw "bad argument in function set_obj_collision_damage_type"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_obj_camera_avoids(lua_State* L) {// камера обходит объект.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на объект.
			const void* p = lua_topointer(L, 1);
			CObject* obj = findobjinpool(p);// получить указатель на объект.
			bool avoids = obj->m_bCameraAvoids;// камера обходит.
			lua_pushboolean(L, avoids);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_obj_camera_avoids"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_obj_camera_avoids(lua_State* L) {// set_obj_camera_avoids(obj, true/false) - камера обходит объект.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {// указатель на объект и булево.
			const void* p = lua_topointer(L, 1);
			CObject* obj = findobjinpool(p);// получить указатель на объект.
			bool avoids = lua_toboolean(L, 2);// обходит или нет.
			obj->m_bCameraAvoids = avoids;// установить обход.
			return 0;
		}
		else { throw "bad argument in function set_obj_camera_avoids"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_obj_timer(lua_State* L) {// получить таймер объекта.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на объект.
			const void* p = lua_topointer(L, 1);
			CObject* obj = findobjinpool(p);// получить указатель на объект.
			int timer = obj->m_dwObjectTimer;// таймер объекта.
			lua_pushinteger(L, timer);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_obj_timer"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_obj_timer(lua_State* L) {// set_obj_timer(obj, ms) - установить таймер объекта (для временных объектов).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на объект и число.
			const void* p = lua_topointer(L, 1);
			CObject* obj = findobjinpool(p);// получить указатель на объект.
			int timer = lua_tointeger(L, 2);// таймер объекта.
			obj->m_dwObjectTimer = timer;// установить таймер.
			return 0;
		}
		else { throw "bad argument in function set_obj_timer"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_obj_ref_model_id(lua_State* L) {// получить id модели на которую ссылается объект.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на объект.
			const void* p = lua_topointer(L, 1);
			CObject* obj = findobjinpool(p);// получить указатель на объект.
			int model = obj->m_wRefModelId;// модель объекта.
			lua_pushinteger(L, model);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_obj_ref_model_id"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int obj_can_be_deleted(lua_State* L) {// можно ли удалить объект.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на объект.
			const void* p = lua_topointer(L, 1);
			CObject* obj = findobjinpool(p);// получить указатель на объект.
			bool can = obj->CanBeDeleted();// можно удалить или нет.
			lua_pushboolean(L, can);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function obj_can_be_deleted"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int obj_damage(lua_State* L) {// obj_damage(obj, damage) - нанести объекту урон.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на объект и число.
			const void* p = lua_topointer(L, 1);
			CObject* obj = findobjinpool(p);// получить указатель на объект.
			float damage = lua_tonumber(L, 2);// урон объекту.
			obj->ObjectDamage(damage);// нанести урон.
			return 0;
		}
		else { throw "bad argument in function obj_damage"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int obj_delete_all_mission(lua_State* L) {// удалить все миссионные объекты.
	try {
		CObject::DeleteAllMissionObjects();// удалить миссионные объекты.
		return 0;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int obj_delete_all_temp(lua_State* L) {// удалить все временные объекты.
	try {
		CObject::DeleteAllTempObjects();// удалить временные объекты.
		return 0;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int obj_delete_all_temp_in_area(lua_State* L) {// obj_delete_all_temp_in_area(x, y, z, radius) - удалить временные объекты в радиусе.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {// четыре числа.
			float x = lua_tonumber(L, 1); float y = lua_tonumber(L, 2); float z = lua_tonumber(L, 3);
			float radius = lua_tonumber(L, 4);// радиус.
			CVector pos = { x, y, z };
			CObject::DeleteAllTempObjectsInArea(pos, radius);// удалить в радиусе.
			return 0;
		}
		else { throw "bad argument in function obj_delete_all_temp_in_area"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_obj_attach_force(lua_State* L) {// получить силу прикрепления объекта.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на объект.
			const void* p = lua_topointer(L, 1);
			CObject* obj = findobjinpool(p);// получить указатель на объект.
			float force = obj->m_fAttachForce;// сила прикрепления.
			lua_pushnumber(L, force);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_obj_attach_force"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_obj_attach_force(lua_State* L) {// set_obj_attach_force(obj, force) - установить силу прикрепления объекта.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на объект и число.
			const void* p = lua_topointer(L, 1);
			CObject* obj = findobjinpool(p);// получить указатель на объект.
			float force = lua_tonumber(L, 2);// сила прикрепления.
			obj->m_fAttachForce = force;// установить силу.
			return 0;
		}
		else { throw "bad argument in function set_obj_attach_force"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int fire_find_nearest(lua_State* L) {// fire_find_nearest(x, y, z) - найти ближайший пожар к точке.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {// три числа.
			float x = lua_tonumber(L, 1); float y = lua_tonumber(L, 2); float z = lua_tonumber(L, 3);
			CVector pos = { x, y, z };
			float range = 0.0f;// дистанция до пожара.
			CFire* fire = gFireManager.FindNearestFire(pos, &range);// найти ближайший пожар.
			if (fire == NULL) { return 0; }// нет пожара - ничего не отправлять.
			lua_pushlightuserdata(L, fire);// отправить указатель на пожар.
			lua_pushnumber(L, range);// отправить дистанцию.
			return 2;
		}
		else { throw "bad argument in function fire_find_nearest"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int fire_start_on_point(lua_State* L) {// fire_start_on_point(x, y, z, size, moves) - поджечь точку.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4) && LUA_TBOOLEAN == lua_type(L, 5)) {// числа и булево.
			float x = lua_tonumber(L, 1); float y = lua_tonumber(L, 2); float z = lua_tonumber(L, 3);
			float size = lua_tonumber(L, 4);// размер пожара.
			bool moves = lua_toboolean(L, 5);// пожар распространяется.
			CVector pos = { x, y, z };
			gFireManager.StartFire(pos, size, moves);// поджечь точку.
			return 0;
		}
		else { throw "bad argument in function fire_start_on_point"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_fire_time_to_burn(lua_State* L) {// получить время горения пожара.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на пожар.
			const void* p = lua_topointer(L, 1);
			CFire* fire = (CFire*)p;// указатель на пожар.
			int time = fire->m_nTimeToBurn;// время горения.
			lua_pushinteger(L, time);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_fire_time_to_burn"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_fire_time_to_burn(lua_State* L) {// set_fire_time_to_burn(fire, ms) - установить время горения пожара.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на пожар и число.
			const void* p = lua_topointer(L, 1);
			CFire* fire = (CFire*)p;// указатель на пожар.
			int time = lua_tointeger(L, 2);// время горения.
			fire->m_nTimeToBurn = time;// установить время.
			return 0;
		}
		else { throw "bad argument in function set_fire_time_to_burn"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_fire_target(lua_State* L) {// получить цель пожара (на чем горит).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на пожар.
			const void* p = lua_topointer(L, 1);
			CFire* fire = (CFire*)p;// указатель на пожар.
			CEntity* target = fire->m_pEntityTarget;// цель пожара.
			if (target == NULL) { return 0; }// нет цели - ничего не отправлять.
			lua_pushlightuserdata(L, target);// отправить указатель на сущность.
			return 1;
		}
		else { throw "bad argument in function get_fire_target"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int fire_is_active(lua_State* L) {// пожар активен.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на пожар.
			const void* p = lua_topointer(L, 1);
			CFire* fire = (CFire*)p;// указатель на пожар.
			bool active = fire->m_bActive;// пожар активен.
			lua_pushboolean(L, active);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function fire_is_active"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_fire_position(lua_State* L) {// получить координаты пожара (x,y,z).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на пожар.
			const void* p = lua_topointer(L, 1);
			CFire* fire = (CFire*)p;// указатель на пожар.
			CVector pos = fire->m_vecPosition;// координаты пожара.
			lua_pushnumber(L, pos.x); lua_pushnumber(L, pos.y); lua_pushnumber(L, pos.z);// отправить в стек.
			return 3;
		}
		else { throw "bad argument in function get_fire_position"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_play_anim(lua_State* L) {// ped_play_anim(ped, group, id) - запустить анимацию педа без смешивания.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {// указатель на педа и два числа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			int group = lua_tointeger(L, 2);// группа анимаций.
			int id = lua_tointeger(L, 3);// номер анимации.
			RpClump* clump = ped->m_pRwClump;
			CAnimManager::AddAnimation(clump, group, id);// запустить анимацию.
			return 0;
		}
		else { throw "bad argument in function ped_play_anim"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int ped_blend_anim(lua_State* L) {// ped_blend_anim(ped, group, id, time) - запустить анимацию педа со смешиванием.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {// указатель на педа и три числа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			int group = lua_tointeger(L, 2);// группа анимаций.
			int id = lua_tointeger(L, 3);// номер анимации.
			float time = lua_tonumber(L, 4);// время смешивания.
			RpClump* clump = ped->m_pRwClump;
			CAnimManager::BlendAnimation(clump, group, id, time);// запустить анимацию со смешиванием.
			return 0;
		}
		else { throw "bad argument in function ped_blend_anim"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_anim_block_index(lua_State* L) {// get_anim_block_index(name) - получить индекс блока анимаций по имени.
	try {
		if (LUA_TSTRING == lua_type(L, 1)) {// строка.
			const char* name = lua_tostring(L, 1);// имя блока.
			int index = CAnimManager::GetAnimationBlockIndex(name);// индекс блока.
			lua_pushinteger(L, index);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function get_anim_block_index"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int anim_set_group_ref(lua_State* L) {// anim_set_group_ref(ped, group) - задать педу группу анимаций (для последующих анимаций).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на педа и число.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			int group = lua_tointeger(L, 2);// группа анимаций.
			ped->m_nAnimGroupId = group;// установить группу.
			return 0;
		}
		else { throw "bad argument in function anim_set_group_ref"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_ped_stat_kills(lua_State* L) {// получить общее число законных убийств игрока.
	try {
		int kills = CStats::TotalLegitimateKills;// законные убийства.
		lua_pushinteger(L, kills);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_ped_stat_cheated(lua_State* L) {// сколько раз игрок читерил.
	try {
		int cheated = CStats::CheatedCount;// число читов.
		lua_pushinteger(L, cheated);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_stat_property_destroyed(lua_State* L) {// получить кол-во разрушенного имущества.
	try {
		int prop = CStats::PropertyDestroyed;// разрушенное имущество.
		lua_pushinteger(L, prop);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_stat_longest_wheelie_dist(lua_State* L) {// получить дистанцию самого длинного вилли.
	try {
		float dist = CStats::LongestWheelieDist;// дистанция вилли.
		lua_pushnumber(L, dist);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_stat_longest_stoppie_dist(lua_State* L) {// получить дистанцию самого длинного стоппи.
	try {
		float dist = CStats::LongestStoppieDist;// дистанция стоппи.
		lua_pushnumber(L, dist);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_stat_longest_2wheel_dist(lua_State* L) {// получить дистанцию самого длинного заезда на двух колесах.
	try {
		float dist = CStats::Longest2WheelDist;// дистанция на двух колесах.
		lua_pushnumber(L, dist);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_stat_longest_wheelie_time(lua_State* L) {// получить время самого длинного вилли.
	try {
		int time = CStats::LongestWheelie;// время вилли.
		lua_pushinteger(L, time);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_stat_seagulls_killed(lua_State* L) {// получить кол-во убитых чаек.
	try {
		int seagulls = CStats::SeagullsKilled;// убитые чайки.
		lua_pushinteger(L, seagulls);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_stat_times_drowned(lua_State* L) {// получить кол-во утоплений игрока.
	try {
		int drowned = CStats::TimesDrowned;// число утоплений.
		lua_pushinteger(L, drowned);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_stat_flight_time(lua_State* L) {// получить общее время полетов игрока.
	try {
		int time = CStats::FlightTime;// время полетов.
		lua_pushinteger(L, time);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_stat_missions_total(lua_State* L) {// получить общее число миссий.
	try {
		int missions = CStats::TotalNumberMissions;// общее миссий.
		lua_pushinteger(L, missions);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_stat_frenzies_total(lua_State* L) {// получить общее число безумий.
	try {
		int frenzies = CStats::TotalNumberKillFrenzies;// общее безумий.
		lua_pushinteger(L, frenzies);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_stat_frenzies_passed(lua_State* L) {// получить число пройденных безумий.
	try {
		int frenzies = CStats::NumberKillFrenziesPassed;// пройденные безумия.
		lua_pushinteger(L, frenzies);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_stat_ice_cream_sold(lua_State* L) {// получить кол-во проданного мороженого.
	try {
		float ice = CStats::IceCreamSold;// проданное мороженое.
		lua_pushnumber(L, ice);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_stat_pizzas_delivered(lua_State* L) {// получить кол-во доставленных пицц.
	try {
		float pizzas = CStats::PizzasDelivered;// доставленные пиццы.
		lua_pushnumber(L, pizzas);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_stat_assassinations(lua_State* L) {// получить кол-во совершенных убийств (заказных).
	try {
		float assass = CStats::Assassinations;// заказные убийства.
		lua_pushnumber(L, assass);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_stat_movie_stunts(lua_State* L) {// получить кол-во кинотрюков.
	try {
		float stunts = CStats::MovieStunts;// кинотрюки.
		lua_pushnumber(L, stunts);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_stat_stores_knocked_off(lua_State* L) {// получить кол-во ограбленных магазинов.
	try {
		float stores = CStats::StoresKnockedOff;// ограбленные магазины.
		lua_pushnumber(L, stores);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_stat_loan_sharks(lua_State* L) {// получить кол-во сделок с ростовщиками.
	try {
		float loans = CStats::LoanSharks;// ростовщики.
		lua_pushnumber(L, loans);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_stat_garbage_pickups(lua_State* L) {// получить кол-во собранного мусора.
	try {
		float garbage = CStats::GarbagePickups;// собранный мусор.
		lua_pushnumber(L, garbage);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_stat_weapon_budget(lua_State* L) {// получить сколько потрачено на оружие.
	try {
		float budget = CStats::WeaponBudget;// бюджет на оружие.
		lua_pushnumber(L, budget);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_stat_fashion_budget(lua_State* L) {// получить сколько потрачено на одежду.
	try {
		float budget = CStats::FashionBudget;// бюджет на одежду.
		lua_pushnumber(L, budget);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_stat_property_budget(lua_State* L) {// получить сколько потрачено на собственность.
	try {
		float budget = CStats::PropertyBudget;// бюджет на собственность.
		lua_pushnumber(L, budget);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_stat_auto_painting_budget(lua_State* L) {// получить сколько потрачено на покраску авто.
	try {
		float budget = CStats::AutoPaintingBudget;// бюджет на покраску.
		lua_pushnumber(L, budget);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_stat_bloodring_kills(lua_State* L) {// получить кол-во убийств на арене Bloodring.
	try {
		int kills = CStats::BloodRingKills;// убийства на арене.
		lua_pushinteger(L, kills);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_stat_bloodring_time(lua_State* L) {// получить время на арене Bloodring.
	try {
		int time = CStats::BloodRingTime;// время на арене.
		lua_pushinteger(L, time);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_stat_shooting_rank(lua_State* L) {// получить ранг стрельбы игрока.
	try {
		float rank = CStats::ShootingRank;// ранг стрельбы.
		lua_pushnumber(L, rank);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_stat_top_shooting_score(lua_State* L) {// получить лучший счет в тире.
	try {
		float score = CStats::TopShootingRangeScore;// лучший счет.
		lua_pushnumber(L, score);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_stat_num_property_owned(lua_State* L) {// получить кол-во купленной собственности.
	try {
		int num = CStats::NumPropertyOwned;// кол-во собственности.
		lua_pushinteger(L, num);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int radar_set_coord_blip(lua_State* L) {// radar_set_coord_blip(type, x, y, z, colour, display) - создать метку на карте (возвращает id).
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4) && LUA_TNUMBER == lua_type(L, 5) && LUA_TNUMBER == lua_type(L, 6)) {// шесть чисел.
			int type = lua_tointeger(L, 1);// тип метки.
			float x = lua_tonumber(L, 2); float y = lua_tonumber(L, 3); float z = lua_tonumber(L, 4);// координаты.
			int colour = lua_tointeger(L, 5);// цвет метки.
			int display = lua_tointeger(L, 6);// показ метки.
			CVector pos = { x, y, z };
			int id = CRadar::SetCoordBlip((eBlipType)type, pos, colour, (eBlipDisplay)display);// создать метку.
			lua_pushinteger(L, id);// отправить id в стек.
			return 1;
		}
		else { throw "bad argument in function radar_set_coord_blip"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int radar_set_short_range_blip(lua_State* L) {// radar_set_short_range_blip(type, x, y, z, colour, display) - короткодействующая метка (возвращает id).
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4) && LUA_TNUMBER == lua_type(L, 5) && LUA_TNUMBER == lua_type(L, 6)) {// шесть чисел.
			int type = lua_tointeger(L, 1);// тип метки.
			float x = lua_tonumber(L, 2); float y = lua_tonumber(L, 3); float z = lua_tonumber(L, 4);// координаты.
			int colour = lua_tointeger(L, 5);// цвет метки.
			int display = lua_tointeger(L, 6);// показ метки.
			CVector pos = { x, y, z };
			int id = CRadar::SetShortRangeCoordBlip((eBlipType)type, pos, colour, (eBlipDisplay)display);// создать короткую метку.
			lua_pushinteger(L, id);// отправить id в стек.
			return 1;
		}
		else { throw "bad argument in function radar_set_short_range_blip"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int radar_set_entity_blip(lua_State* L) {// radar_set_entity_blip(type, entity, colour, display) - создать метку на сущности (пед/авто/объект).
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {// тип, указатель, два числа.
			int type = lua_tointeger(L, 1);// тип метки.
			int colour = lua_tointeger(L, 3);// цвет метки.
			int display = lua_tointeger(L, 4);// показ метки.
			const void* p = lua_topointer(L, 2);// указатель на сущность.
			CEntity* entity = (CEntity*)p;// сущность.
			int handle = 0;// хэндл сущности.
			// определить тип сущности и взять хэндл.
			if (findpedinpool(p) != NULL) { handle = CPools::GetPedRef(findpedinpool(p)); }
			else if (findcarinpool(p) != NULL) { handle = CPools::GetVehicleRef(findcarinpool(p)); }
			else if (findobjinpool(p) != NULL) { handle = CPools::GetObjectRef(findobjinpool(p)); }
			else { throw "entity not found in function radar_set_entity_blip"; }
			CRadar::SetEntityBlip((eBlipType)type, handle, colour, (eBlipDisplay)display);// создать метку.
			return 0;
		}
		else { throw "bad argument in function radar_set_entity_blip"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int radar_clear_blip(lua_State* L) {// radar_clear_blip(id) - удалить метку с карты.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// число.
			int id = lua_tointeger(L, 1);// id метки.
			CRadar::ClearBlip(id);// удалить метку.
			return 0;
		}
		else { throw "bad argument in function radar_clear_blip"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int radar_change_blip_colour(lua_State* L) {// radar_change_blip_colour(id, colour) - изменить цвет метки.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// два числа.
			int id = lua_tointeger(L, 1);// id метки.
			int colour = lua_tointeger(L, 2);// цвет метки.
			CRadar::ChangeBlipColour(id, colour);// изменить цвет.
			return 0;
		}
		else { throw "bad argument in function radar_change_blip_colour"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int radar_change_blip_brightness(lua_State* L) {// radar_change_blip_brightness(id, bright) - изменить яркость метки.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// два числа.
			int id = lua_tointeger(L, 1);// id метки.
			int bright = lua_tointeger(L, 2);// яркость метки.
			CRadar::ChangeBlipBrightness(id, bright);// изменить яркость.
			return 0;
		}
		else { throw "bad argument in function radar_change_blip_brightness"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int radar_change_blip_scale(lua_State* L) {// radar_change_blip_scale(id, scale) - изменить размер метки.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// два числа.
			int id = lua_tointeger(L, 1);// id метки.
			int scale = lua_tointeger(L, 2);// размер метки.
			CRadar::ChangeBlipScale(id, scale);// изменить размер.
			return 0;
		}
		else { throw "bad argument in function radar_change_blip_scale"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int radar_change_blip_display(lua_State* L) {// radar_change_blip_display(id, display) - изменить показ метки (0-не показывать,1-только карта,2-карта и радар).
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// два числа.
			int id = lua_tointeger(L, 1);// id метки.
			int display = lua_tointeger(L, 2);// показ метки.
			CRadar::ChangeBlipDisplay(id, (eBlipDisplay)display);// изменить показ.
			return 0;
		}
		else { throw "bad argument in function radar_change_blip_display"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int radar_set_blip_sprite(lua_State* L) {// radar_set_blip_sprite(id, icon) - установить иконку метки.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// два числа.
			int id = lua_tointeger(L, 1);// id метки.
			int icon = lua_tointeger(L, 2);// иконка метки.
			CRadar::SetBlipSprite(id, icon);// установить иконку.
			return 0;
		}
		else { throw "bad argument in function radar_set_blip_sprite"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int radar_show_marker(lua_State* L) {// radar_show_marker(x, y, z, colour, radius) - показать круглый маркер в мире.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4) && LUA_TNUMBER == lua_type(L, 5)) {// пять чисел.
			float x = lua_tonumber(L, 1); float y = lua_tonumber(L, 2); float z = lua_tonumber(L, 3);// координаты.
			int colour = lua_tointeger(L, 4);// цвет маркера.
			float radius = lua_tonumber(L, 5);// радиус маркера.
			CVector pos = { x, y, z };
			CRadar::ShowRadarMarker(pos, colour, radius);// показать маркер.
			return 0;
		}
		else { throw "bad argument in function radar_show_marker"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int radar_set_marker_state(lua_State* L) {// radar_set_marker_state(id, flag) - установить состояние маркера миссии.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// два числа.
			int id = lua_tointeger(L, 1);// id метки.
			int flag = lua_tointeger(L, 2);// состояние маркера.
			CRadar::SetRadarMarkerState(id, flag);// установить состояние.
			return 0;
		}
		else { throw "bad argument in function radar_set_marker_state"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int radar_draw_radar_sprite(lua_State* L) {// radar_draw_radar_sprite(sprite, x, y, alpha) - отрисовать спрайт на радаре (в экранных координатах).
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {// четыре числа.
			int sprite = lua_tointeger(L, 1);// номер спрайта.
			float x = lua_tonumber(L, 2); float y = lua_tonumber(L, 3);// координаты экрана.
			int alpha = lua_tointeger(L, 4);// прозрачность.
			CRadar::DrawRadarSprite(sprite, x, y, alpha);// отрисовать спрайт.
			return 0;
		}
		else { throw "bad argument in function radar_draw_radar_sprite"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int radar_get_radar_range(lua_State* L) {// получить дальность радара.
	try {
		float range = CRadar::m_radarRange;// дальность радара.
		lua_pushnumber(L, range);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int radar_set_radar_range(lua_State* L) {// radar_set_radar_range(range) - установить дальность радара.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// число.
			float range = lua_tonumber(L, 1);// дальность радара.
			CRadar::m_radarRange = range;// установить дальность.
			return 0;
		}
		else { throw "bad argument in function radar_set_radar_range"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int radar_clear_blip_for_entity(lua_State* L) {// radar_clear_blip_for_entity(type, entity) - удалить метку сущности.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {// тип и указатель.
			int type = lua_tointeger(L, 1);// тип метки.
			const void* p = lua_topointer(L, 2);// указатель на сущность.
			int handle = 0;// хэндл сущности.
			if (findpedinpool(p) != NULL) { handle = CPools::GetPedRef(findpedinpool(p)); }
			else if (findcarinpool(p) != NULL) { handle = CPools::GetVehicleRef(findcarinpool(p)); }
			else if (findobjinpool(p) != NULL) { handle = CPools::GetObjectRef(findobjinpool(p)); }
			else { throw "entity not found in function radar_clear_blip_for_entity"; }
			CRadar::ClearBlipForEntity((eBlipType)type, handle);// удалить метку.
			return 0;
		}
		else { throw "bad argument in function radar_clear_blip_for_entity"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int play_one_off_sound(lua_State* L) {// play_one_off_sound(x, y, z, sound) - проиграть одноразовый звук на координатах.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {// четыре числа.
			float x = lua_tonumber(L, 1); float y = lua_tonumber(L, 2); float z = lua_tonumber(L, 3);// координаты.
			int sound = lua_tointeger(L, 4);// номер звука.
			Command<COMMAND_ADD_ONE_OFF_SOUND>(x, y, z, sound);// проиграть звук.
			return 0;
		}
		else { throw "bad argument in function play_one_off_sound"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int play_continuous_sound(lua_State* L) {// play_continuous_sound(x, y, z, sound) - проиграть непрерывный звук на координатах (возвращает id).
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {// четыре числа.
			float x = lua_tonumber(L, 1); float y = lua_tonumber(L, 2); float z = lua_tonumber(L, 3);// координаты.
			int sound = lua_tointeger(L, 4);// номер звука.
			int id = 0;// id звука.
			Command<COMMAND_ADD_CONTINUOUS_SOUND>(x, y, z, sound, &id);// непрерывный звук.
			if (id != 0) { soundsids.emplace(id, L);// добавить в map звуков (для destroy).
			}
			lua_pushinteger(L, id);// отправить id в стек.
			return 1;
		}
		else { throw "bad argument in function play_continuous_sound"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int remove_sound_by_id(lua_State* L) {// remove_sound_by_id(id) - удалить непрерывный звук.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// число.
			int id = lua_tointeger(L, 1);// id звука.
			Command<COMMAND_REMOVE_SOUND>(id);// удалить звук.
			auto it = soundsids.find(id);// убрать из списка звуков.
			if (it != soundsids.end() && L == it->second) {
				soundsids.erase(it);
			}
			return 0;
		}
		else { throw "bad argument in function remove_sound_by_id"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_radio_channel(lua_State* L) {// set_radio_channel(station) - установить радиостанцию игроку (0-9).
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// число.
			int station = lua_tointeger(L, 1);// номер станции.
			Command<COMMAND_SET_RADIO_CHANNEL>(station);// установить станцию.
			return 0;
		}
		else { throw "bad argument in function set_radio_channel"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_radio_channel(lua_State* L) {// получить текущую радиостанцию игрока.
	try {
		int station = 0;// номер станции.
		Command<COMMAND_GET_RADIO_CHANNEL>(&station);// получить станцию.
		lua_pushinteger(L, station);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_music_does_fade(lua_State* L) {// set_music_does_fade(true/false) - музыка затухает при выходе из авто.
	try {
		if (LUA_TBOOLEAN == lua_type(L, 1)) {// булево.
			bool fade = lua_toboolean(L, 1);// затухание.
			Command<COMMAND_SET_MUSIC_DOES_FADE>(fade);// установить затухание.
			return 0;
		}
		else { throw "bad argument in function set_music_does_fade"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int police_radio_message(lua_State* L) {// police_radio_message(type) - передать сообщение в полицейское радио (0-пехота,1-авто).
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// число.
			int type = lua_tointeger(L, 1);// тип сообщения.
			Command<COMMAND_POLICE_RADIO_MESSAGE>(type);// передать сообщение.
			return 0;
		}
		else { throw "bad argument in function police_radio_message"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int create_explosion_no_sound(lua_State* L) {// create_explosion_no_sound(type, x, y, z) - создать взрыв без звука.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {// четыре числа.
			int type = lua_tointeger(L, 1);// тип взрыва.
			float x = lua_tonumber(L, 2); float y = lua_tonumber(L, 3); float z = lua_tonumber(L, 4);// координаты.
			Command<COMMAND_ADD_EXPLOSION_NO_SOUND>(x, y, z, type);// создать взрыв.
			return 0;
		}
		else { throw "bad argument in function create_explosion_no_sound"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_ped_density_multiplier(lua_State* L) {// получить множитель плотности пешеходов.
	try {
		float density = CPopulation::PedDensityMultiplier;// плотность педов.
		lua_pushnumber(L, density);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_ped_density_multiplier(lua_State* L) {// set_ped_density_multiplier(1.0) - установить множитель плотности пешеходов (0.0 - никого).
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// число.
			float density = lua_tonumber(L, 1);// плотность педов.
			CPopulation::PedDensityMultiplier = density;// установить плотность.
			return 0;
		}
		else { throw "bad argument in function set_ped_density_multiplier"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_all_random_peds_type(lua_State* L) {// получить тип всех случайных педов.
	try {
		int type = CPopulation::m_AllRandomPedsThisType;// тип случайных педов.
		lua_pushinteger(L, type);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_all_random_peds_type(lua_State* L) {// set_all_random_peds_type(type) - установить тип всех случайных педов.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// число.
			int type = lua_tointeger(L, 1);// тип случайных педов.
			CPopulation::m_AllRandomPedsThisType = type;// установить тип.
			return 0;
		}
		else { throw "bad argument in function set_all_random_peds_type"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int population_manage(lua_State* L) {// принудительно обработать популяцию педов.
	try {
		CPopulation::ManagePopulation();// обработать популяцию.
		return 0;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int population_remove_peds_full_pool(lua_State* L) {// удалить педов если пул переполнен.
	try {
		CPopulation::RemovePedsIfThePoolGetsFull();// удалить лишних педов.
		return 0;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int population_convert_all_objects_to_dummy(lua_State* L) {// конвертировать все объекты в dummy (оптимизация).
	try {
		CPopulation::ConvertAllObjectsToDummyObjects();// конвертировать объекты.
		return 0;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int population_is_female(lua_State* L) {// population_is_female(model) - модель педа женская.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// число.
			int model = lua_tointeger(L, 1);// модель педа.
			bool female = CPopulation::IsFemale(model);// женская модель.
			lua_pushboolean(L, female);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function population_is_female"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int population_is_male(lua_State* L) {// population_is_male(model) - модель педа мужская.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// число.
			int model = lua_tointeger(L, 1);// модель педа.
			bool male = CPopulation::IsMale(model);// мужская модель.
			lua_pushboolean(L, male);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function population_is_male"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int population_is_sunbather(lua_State* L) {// population_is_sunbather(model) - модель педа загорающий.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// число.
			int model = lua_tointeger(L, 1);// модель педа.
			bool sun = CPopulation::IsSunbather(model);// загорающая модель.
			lua_pushboolean(L, sun);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function population_is_sunbather"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int world_find_ground_z_for_coord(lua_State* L) {// world_find_ground_z_for_coord(x, y) - получить высоту земли под точкой.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// два числа.
			float x = lua_tonumber(L, 1); float y = lua_tonumber(L, 2);// координаты.
			float z = CWorld::FindGroundZForCoord(x, y);// высота земли.
			lua_pushnumber(L, z);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function world_find_ground_z_for_coord"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int world_find_ground_z_for_3d_coord(lua_State* L) {// world_find_ground_z_for_3d_coord(x, y, z) - получить высоту земли под 3d точкой.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {// три числа.
			float x = lua_tonumber(L, 1); float y = lua_tonumber(L, 2); float z = lua_tonumber(L, 3);// координаты.
			bool result = false;// результат проверки.
			float ground = CWorld::FindGroundZFor3DCoord(x, y, z, &result);// высота земли.
			lua_pushnumber(L, ground);// отправить высоту.
			lua_pushboolean(L, result);// отправить результат.
			return 2;
		}
		else { throw "bad argument in function world_find_ground_z_for_3d_coord"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int world_find_roof_z_for_3d_coord(lua_State* L) {// world_find_roof_z_for_3d_coord(x, y, z) - получить высоту крыши под точкой.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {// три числа.
			float x = lua_tonumber(L, 1); float y = lua_tonumber(L, 2); float z = lua_tonumber(L, 3);// координаты.
			bool result = false;// результат проверки.
			float roof = CWorld::FindRoofZFor3DCoord(x, y, z, &result);// высота крыши.
			lua_pushnumber(L, roof);// отправить высоту.
			lua_pushboolean(L, result);// отправить результат.
			return 2;
		}
		else { throw "bad argument in function world_find_roof_z_for_3d_coord"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int world_get_is_line_of_sight_clear(lua_State* L) {// world_get_is_line_of_sight_clear(x1,y1,z1,x2,y2,z2) - есть ли прямая видимость между точками.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4) && LUA_TNUMBER == lua_type(L, 5) && LUA_TNUMBER == lua_type(L, 6)) {// шесть чисел.
			float x1 = lua_tonumber(L, 1); float y1 = lua_tonumber(L, 2); float z1 = lua_tonumber(L, 3);// первая точка.
			float x2 = lua_tonumber(L, 4); float y2 = lua_tonumber(L, 5); float z2 = lua_tonumber(L, 6);// вторая точка.
			CVector origin = { x1, y1, z1 };
			CVector target = { x2, y2, z2 };
			bool clear = CWorld::GetIsLineOfSightClear(origin, target, true, true, true, true, true, false, false);// видимость.
			lua_pushboolean(L, clear);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function world_get_is_line_of_sight_clear"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int world_clear_peds_from_area(lua_State* L) {// world_clear_peds_from_area(x1,y1,z1,x2,y2,z2) - удалить педов из области.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4) && LUA_TNUMBER == lua_type(L, 5) && LUA_TNUMBER == lua_type(L, 6)) {// шесть чисел.
			float x1 = lua_tonumber(L, 1); float y1 = lua_tonumber(L, 2); float z1 = lua_tonumber(L, 3);// первый угол.
			float x2 = lua_tonumber(L, 4); float y2 = lua_tonumber(L, 5); float z2 = lua_tonumber(L, 6);// второй угол.
			CWorld::ClearPedsFromArea(x1, y1, z1, x2, y2, z2);// удалить педов.
			return 0;
		}
		else { throw "bad argument in function world_clear_peds_from_area"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int world_clear_cars_from_area(lua_State* L) {// world_clear_cars_from_area(x1,y1,z1,x2,y2,z2) - удалить авто из области.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4) && LUA_TNUMBER == lua_type(L, 5) && LUA_TNUMBER == lua_type(L, 6)) {// шесть чисел.
			float x1 = lua_tonumber(L, 1); float y1 = lua_tonumber(L, 2); float z1 = lua_tonumber(L, 3);// первый угол.
			float x2 = lua_tonumber(L, 4); float y2 = lua_tonumber(L, 5); float z2 = lua_tonumber(L, 6);// второй угол.
			CWorld::ClearCarsFromArea(x1, y1, z1, x2, y2, z2);// удалить авто.
			return 0;
		}
		else { throw "bad argument in function world_clear_cars_from_area"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int world_extinguish_all_car_fires_in_area(lua_State* L) {// world_extinguish_all_car_fires_in_area(x, y, z, radius) - потушить все пожары авто в радиусе.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {// четыре числа.
			float x = lua_tonumber(L, 1); float y = lua_tonumber(L, 2); float z = lua_tonumber(L, 3);// координаты.
			float radius = lua_tonumber(L, 4);// радиус.
			CVector pos = { x, y, z };
			CWorld::ExtinguishAllCarFiresInArea(pos, radius);// потушить пожары.
			return 0;
		}
		else { throw "bad argument in function world_extinguish_all_car_fires_in_area"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int world_clear_exciting_stuff_from_area(lua_State* L) {// world_clear_exciting_stuff_from_area(x, y, z, radius) - очистить_area от взрывов/пуль/эффектов.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {// четыре числа.
			float x = lua_tonumber(L, 1); float y = lua_tonumber(L, 2); float z = lua_tonumber(L, 3);// координаты.
			float radius = lua_tonumber(L, 4);// радиус.
			CVector pos = { x, y, z };
			CWorld::ClearExcitingStuffFromArea(pos, radius, false);// очистить область.
			return 0;
		}
		else { throw "bad argument in function world_clear_exciting_stuff_from_area"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int world_call_off_chase_for_area(lua_State* L) {// world_call_off_chase_for_area(x1,y1,x2,y2) - отменить погоню в области.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {// четыре числа.
			float x1 = lua_tonumber(L, 1); float y1 = lua_tonumber(L, 2);// первый угол.
			float x2 = lua_tonumber(L, 3); float y2 = lua_tonumber(L, 4);// второй угол.
			CWorld::CallOffChaseForArea(x1, y1, x2, y2);// отменить погоню.
			return 0;
		}
		else { throw "bad argument in function world_call_off_chase_for_area"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int world_add_entity(lua_State* L) {// world_add_entity(entity) - добавить сущность в мир.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на сущность.
			const void* p = lua_topointer(L, 1);
			CEntity* entity = (CEntity*)p;// сущность.
			CWorld::Add(entity);// добавить в мир.
			return 0;
		}
		else { throw "bad argument in function world_add_entity"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int world_clear_scan_codes(lua_State* L) {// сбросить коды сканирования мира.
	try {
		CWorld::ClearScanCodes();// сбросить коды.
		return 0;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int general_atan_of_xy(lua_State* L) {// general_atan_of_xy(x, y) - получить арктангенс от x,y в градусах.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// два числа.
			float x = lua_tonumber(L, 1); float y = lua_tonumber(L, 2);// координаты.
			float angle = CGeneral::GetATanOfXY(x, y);// арктангенс.
			lua_pushnumber(L, angle);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function general_atan_of_xy"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int general_angle_between_points(lua_State* L) {// general_angle_between_points(x1,y1,x2,y2) - угол между точками в градусах.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {// четыре числа.
			float x1 = lua_tonumber(L, 1); float y1 = lua_tonumber(L, 2);// первая точка.
			float x2 = lua_tonumber(L, 3); float y2 = lua_tonumber(L, 4);// вторая точка.
			float angle = CGeneral::GetAngleBetweenPoints(x1, y1, x2, y2);// угол между точками.
			lua_pushnumber(L, angle);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function general_angle_between_points"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int general_radian_angle_between_points(lua_State* L) {// general_radian_angle_between_points(x1,y1,x2,y2) - угол между точками в радианах.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {// четыре числа.
			float x1 = lua_tonumber(L, 1); float y1 = lua_tonumber(L, 2);// первая точка.
			float x2 = lua_tonumber(L, 3); float y2 = lua_tonumber(L, 4);// вторая точка.
			float angle = CGeneral::GetRadianAngleBetweenPoints(x1, y1, x2, y2);// угол между точками.
			lua_pushnumber(L, angle);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function general_radian_angle_between_points"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int general_limit_angle(lua_State* L) {// general_limit_angle(angle) - нормализовать угол в градусах (0-360).
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// число.
			float angle = lua_tonumber(L, 1);// угол.
			float limited = CGeneral::LimitAngle(angle);// нормализованный угол.
			lua_pushnumber(L, limited);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function general_limit_angle"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int general_limit_radian_angle(lua_State* L) {// general_limit_radian_angle(angle) - нормализовать угол в радианах (0-2пи).
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// число.
			float angle = lua_tonumber(L, 1);// угол.
			float limited = CGeneral::LimitRadianAngle(angle);// нормализованный угол.
			lua_pushnumber(L, limited);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function general_limit_radian_angle"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int general_node_heading_from_vector(lua_State* L) {// general_node_heading_from_vector(x, y) - получить направление из вектора.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// два числа.
			float x = lua_tonumber(L, 1); float y = lua_tonumber(L, 2);// вектор.
			int heading = CGeneral::GetNodeHeadingFromVector(x, y);// направление.
			lua_pushinteger(L, heading);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function general_node_heading_from_vector"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_timer_ms(lua_State* L) {// получить игровое время в миллисекундах.
	try {
		int time = CTimer::m_snTimeInMilliseconds;// игровое время.
		lua_pushinteger(L, time);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_timer_ms_pause_mode(lua_State* L) {// получить игровое время в миллисекундах с учетом паузы.
	try {
		int time = CTimer::m_snTimeInMillisecondsPauseMode;// время с паузой.
		lua_pushinteger(L, time);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_frame_counter(lua_State* L) {// получить номер текущего кадра.
	try {
		int frame = CTimer::m_FrameCounter;// номер кадра.
		lua_pushinteger(L, frame);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int get_time_step(lua_State* L) {// получить шаг времени кадра.
	try {
		float step = CTimer::ms_fTimeStep;// шаг времени.
		lua_pushnumber(L, step);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int is_user_pause(lua_State* L) {// игра на паузе пользователем.
	try {
		bool pause = CTimer::m_UserPause;// пауза.
		lua_pushboolean(L, pause);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int gangs_set_weapons(lua_State* L) {// gangs_set_weapons(gang, weapon1, weapon2) - установить оружие банды.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {// три числа.
			int gang = lua_tointeger(L, 1);// номер банды.
			int weapon1 = lua_tointeger(L, 2);// первое оружие.
			int weapon2 = lua_tointeger(L, 3);// второе оружие.
			CGangs::SetGangWeapons(gang, weapon1, weapon2);// установить оружие.
			return 0;
		}
		else { throw "bad argument in function gangs_set_weapons"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int gangs_set_vehicle_model(lua_State* L) {// gangs_set_vehicle_model(gang, model) - установить авто банды.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// два числа.
			int gang = lua_tointeger(L, 1);// номер банды.
			int model = lua_tointeger(L, 2);// модель авто.
			CGangs::SetGangVehicleModel(gang, model);// установить авто.
			return 0;
		}
		else { throw "bad argument in function gangs_set_vehicle_model"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int gangs_set_ped_models(lua_State* L) {// gangs_set_ped_models(gang, model1, model2) - установить модели педов банды.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {// три числа.
			int gang = lua_tointeger(L, 1);// номер банды.
			int model1 = lua_tointeger(L, 2);// первая модель.
			int model2 = lua_tointeger(L, 3);// вторая модель.
			CGangs::SetGangPedModels(gang, model1, model2);// установить модели.
			return 0;
		}
		else { throw "bad argument in function gangs_set_ped_models"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int gangs_choose_ped_model(lua_State* L) {// gangs_choose_ped_model(gang) - получить случайную модель педа банды.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// число.
			int gang = lua_tointeger(L, 1);// номер банды.
			int model = CGangs::ChooseGangPedModel(gang);// модель педа.
			lua_pushinteger(L, model);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function gangs_choose_ped_model"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int gangs_set_attack_with_cops(lua_State* L) {// gangs_set_attack_with_cops(pedtype, true/false) - банда атакует игрока с копами.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {// число и булево.
			int pedtype = lua_tointeger(L, 1);// тип педа.
			bool attack = lua_toboolean(L, 2);// атаковать или нет.
			CGangs::SetWillAttackPlayerWithCops((ePedType)pedtype, attack);// установить атаку.
			return 0;
		}
		else { throw "bad argument in function gangs_set_attack_with_cops"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int gangs_get_attack_with_cops(lua_State* L) {// gangs_get_attack_with_cops(pedtype) - банда атакует игрока с копами.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// число.
			int pedtype = lua_tointeger(L, 1);// тип педа.
			bool attack = CGangs::GetWillAttackPlayerWithCops((ePedType)pedtype);// атакует или нет.
			lua_pushboolean(L, attack);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function gangs_get_attack_with_cops"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int gangs_initialise(lua_State* L) {// переинициализировать данные банд.
	try {
		CGangs::Initialise();// инициализация банд.
		return 0;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int gamelogic_pass_time(lua_State* L) {// gamelogic_pass_time(time) - пропустить игровое время (мс).
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// число.
			int time = lua_tointeger(L, 1);// время в мс.
			CGameLogic::PassTime(time);// пропустить время.
			return 0;
		}
		else { throw "bad argument in function gamelogic_pass_time"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int gamelogic_update(lua_State* L) {// обновить игровую логику.
	try {
		CGameLogic::Update();// обновить логику.
		return 0;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int gamelogic_after_death_arrest_shortcut(lua_State* L) {// настроить такси-шорткат после смерти/ареста.
	try {
		CGameLogic::AfterDeathArrestSetUpShortCutTaxi();// настроить такси.
		return 0;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int message_add_big(lua_State* L) {// message_add_big(text, time, style) - вывести большое сообщение (стайл-заголовок миссии).
	try {
		if (LUA_TSTRING == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {// строка и два числа.
			const char* text = lua_tostring(L, 1);// текст сообщения.
			int time = lua_tointeger(L, 2);// время показа.
			int style = lua_tointeger(L, 3);// стиль сообщения.
			wchar_t* wtext = getwchat(text);// перевод в широкую строку.
			CMessages::AddBigMessage(wtext, time, style);// большое сообщение.
			return 0;
		}
		else { throw "bad argument in function message_add_big"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int message_add_big_queue(lua_State* L) {// message_add_big_queue(text, time, style) - вывести большое сообщение в очередь.
	try {
		if (LUA_TSTRING == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {// строка и два числа.
			const char* text = lua_tostring(L, 1);// текст сообщения.
			int time = lua_tointeger(L, 2);// время показа.
			int style = lua_tointeger(L, 3);// стиль сообщения.
			wchar_t* wtext = getwchat(text);// перевод в широкую строку.
			CMessages::AddBigMessageQ(wtext, time, style);// большое сообщение в очередь.
			return 0;
		}
		else { throw "bad argument in function message_add_big_queue"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int message_add(lua_State* L) {// message_add(text, time, flag) - вывести маленькое сообщение (помощь).
	try {
		if (LUA_TSTRING == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {// строка и два числа.
			const char* text = lua_tostring(L, 1);// текст сообщения.
			int time = lua_tointeger(L, 2);// время показа.
			int flag = lua_tointeger(L, 3);// флаг показа.
			wchar_t* wtext = getwchat(text);// перевод в широкую строку.
			CMessages::AddMessage(wtext, time, flag);// маленькое сообщение.
			return 0;
		}
		else { throw "bad argument in function message_add"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int message_add_jump_queue(lua_State* L) {// message_add_jump_queue(text, time, flag) - вывести маленькое сообщение вне очереди.
	try {
		if (LUA_TSTRING == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {// строка и два числа.
			const char* text = lua_tostring(L, 1);// текст сообщения.
			int time = lua_tointeger(L, 2);// время показа.
			int flag = lua_tointeger(L, 3);// флаг показа.
			wchar_t* wtext = getwchat(text);// перевод в широкую строку.
			CMessages::AddMessageJumpQ(wtext, time, flag);// сообщение вне очереди.
			return 0;
		}
		else { throw "bad argument in function message_add_jump_queue"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int message_add_with_number(lua_State* L) {// message_add_with_number(text, time, flag, number) - вывести сообщение с числом.
	try {
		if (LUA_TSTRING == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {// строка и три числа.
			const char* text = lua_tostring(L, 1);// текст сообщения.
			int time = lua_tointeger(L, 2);// время показа.
			int flag = lua_tointeger(L, 3);// флаг показа.
			int number = lua_tointeger(L, 4);// число в сообщении.
			wchar_t* wtext = getwchat(text);// перевод в широкую строку.
			CMessages::AddMessageWithNumber(wtext, time, flag, number, 0, 0, 0, 0, 0);// сообщение с числом.
			return 0;
		}
		else { throw "bad argument in function message_add_with_number"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int message_clear_all(lua_State* L) {// очистить все сообщения на экране.
	try {
		CMessages::ClearAllMessagesDisplayedByGame();// очистить сообщения.
		return 0;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int message_clear_small_only(lua_State* L) {// очистить только маленькие сообщения.
	try {
		CMessages::ClearSmallMessagesOnly();// очистить маленькие.
		return 0;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int message_clear_this_big_print(lua_State* L) {// message_clear_this_big_print(text) - удалить конкретное большое сообщение.
	try {
		if (LUA_TSTRING == lua_type(L, 1)) {// строка.
			const char* text = lua_tostring(L, 1);// текст сообщения.
			wchar_t* wtext = getwchat(text);// перевод в широкую строку.
			CMessages::ClearThisBigPrint(wtext);// удалить сообщение.
			return 0;
		}
		else { throw "bad argument in function message_clear_this_big_print"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int message_clear_this_print(lua_State* L) {// message_clear_this_print(text) - удалить конкретное маленькое сообщение.
	try {
		if (LUA_TSTRING == lua_type(L, 1)) {// строка.
			const char* text = lua_tostring(L, 1);// текст сообщения.
			wchar_t* wtext = getwchat(text);// перевод в широкую строку.
			CMessages::ClearThisPrint(wtext);// удалить сообщение.
			return 0;
		}
		else { throw "bad argument in function message_clear_this_print"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int font_set_style(lua_State* L) {// font_set_style(style) - установить стиль шрифта (0-3).
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// число.
			int style = lua_tointeger(L, 1);// стиль шрифта.
			CFont::SetFontStyle(style);// установить стиль.
			return 0;
		}
		else { throw "bad argument in function font_set_style"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int font_set_drop_shadow(lua_State* L) {// font_set_drop_shadow(position) - установить позицию тени шрифта.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// число.
			int position = lua_tointeger(L, 1);// позиция тени.
			CFont::SetDropShadowPosition(position);// установить тень.
			return 0;
		}
		else { throw "bad argument in function font_set_drop_shadow"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int font_set_alpha_fade(lua_State* L) {// font_set_alpha_fade(fade) - установить прозрачность шрифта (0.0-1.0).
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// число.
			float fade = lua_tonumber(L, 1);// прозрачность.
			CFont::SetAlphaFade(fade);// установить прозрачность.
			return 0;
		}
		else { throw "bad argument in function font_set_alpha_fade"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int font_set_centre_size(lua_State* L) {// font_set_centre_size(size) - установить размер центр. текста.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// число.
			float size = lua_tonumber(L, 1);// размер текста.
			CFont::SetCentreSize(size);// установить размер.
			return 0;
		}
		else { throw "bad argument in function font_set_centre_size"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int font_set_wrapx(lua_State* L) {// font_set_wrapx(wrap) - установить ширину переноса шрифта.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {// число.
			float wrap = lua_tonumber(L, 1);// ширина переноса.
			CFont::SetWrapx(wrap);// установить ширину.
			return 0;
		}
		else { throw "bad argument in function font_set_wrapx"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int font_set_prop_on(lua_State* L) {// включить пропорциональный шрифт.
	try {
		CFont::SetPropOn();// включить пропорц. шрифт.
		return 0;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int font_set_prop_off(lua_State* L) {// выключить пропорциональный шрифт.
	try {
		CFont::SetPropOff();// выключить пропорц. шрифт.
		return 0;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int font_set_background_on(lua_State* L) {// включить фон шрифта.
	try {
		CFont::SetBackgroundOn();// включить фон.
		return 0;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int font_set_background_off(lua_State* L) {// выключить фон шрифта.
	try {
		CFont::SetBackgroundOff();// выключить фон.
		return 0;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int font_set_right_justify_on(lua_State* L) {// включить выравнивание по правому краю.
	try {
		CFont::SetRightJustifyOn();// включить выравнивание.
		return 0;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int font_set_right_justify_off(lua_State* L) {// выключить выравнивание по правому краю.
	try {
		CFont::SetRightJustifyOff();// выключить выравнивание.
		return 0;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

// ================= ЭТАП 11: Транспорт + краны/зоны/эскалаторы =================

int heli_set_orientation(lua_State* L) {// установить ориентацию вертолета.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			if (car && car->m_nVehicleClass == VEHICLE_HELI) {
				((CHeli*)car)->m_fRotationX = lua_tonumber(L, 2);// установить ориентацию.
				return 0;
			}
			return 0;
		}
		else { throw "bad argument in function heli_set_orientation"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int heli_clear_orientation(lua_State* L) {// очистить ориентацию вертолета.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			if (car && car->m_nVehicleClass == VEHICLE_HELI) {
				((CHeli*)car)->m_fRotationX = 0.0f;// сбросить ориентацию.
				return 0;
			}
			return 0;
		}
		else { throw "bad argument in function heli_clear_orientation"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int heli_set_stabiliser(lua_State* L) {// установить стабилизатор вертолета.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			if (car && car->m_nVehicleClass == VEHICLE_HELI) {
				((CHeli*)car)->field_2D0 = lua_tonumber(L, 2);// установить стабилизатор.
				return 0;
			}
			return 0;
		}
		else { throw "bad argument in function heli_set_stabiliser"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int heli_make_crashing_down(lua_State* L) {// вертолет падает.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			if (car && car->m_nVehicleClass == VEHICLE_HELI) {
				((CHeli*)car)->m_nHeliState = 3;// вертолет падает (HELI_STATE_EXPLODE).
				return 0;
			}
			return 0;
		}
		else { throw "bad argument in function heli_make_crashing_down"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int heli_is_player_in_any_heli(lua_State* L) {// игрок в любом вертолете?.
	try {
		int result = 0;
		Command<COMMAND_IS_PLAYER_IN_ANY_HELI>(&result);// игрок в вертолете.
		lua_pushinteger(L, result);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int heli_is_char_in_any_heli(lua_State* L) {// пед в любом вертолете?.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			int result = 0;
			Command<COMMAND_IS_CHAR_IN_ANY_HELI>(CPools::GetPedRef(ped), &result);// пед в вертолете.
			lua_pushinteger(L, result);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function heli_is_char_in_any_heli"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int heli_switch(lua_State* L) {// переключить вертолет.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			if (car && car->m_nVehicleClass == VEHICLE_HELI) {
				Command<COMMAND_SWITCH_HELICOPTER>(CPools::GetVehicleRef(car));// переключить вертолет.
			}
			return 0;
		}
		else { throw "bad argument in function heli_switch"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int catalina_heli_start(lua_State* L) {// начать вертолет Catalina.
	try {
		Command<COMMAND_START_CATALINA_HELI>();// начать Catalina.
		return 0;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int catalina_heli_take_off(lua_State* L) {// Catalina взлетает.
	try {
		Command<COMMAND_CATALINA_HELI_TAKE_OFF>();// взлетать.
		return 0;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int catalina_heli_remove(lua_State* L) {// удалить Catalina.
	try {
		Command<COMMAND_REMOVE_CATALINA_HELI>();// удалить.
		return 0;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int catalina_heli_grab(lua_State* L) {// сесть в Catalina.
	try {
		Command<COMMAND_GRAB_CATALINA_HELI>();// сесть.
		return 0;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int catalina_heli_fly_away(lua_State* L) {// Catalina улетает.
	try {
		Command<COMMAND_CATALINA_HELI_FLY_AWAY>();// улететь.
		return 0;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int catalina_heli_has_shot_down(lua_State* L) {// Catalina сбит?.
	try {
		int result = 0;
		Command<COMMAND_HAS_CATALINA_HELI_BEEN_SHOT_DOWN>(&result);// сбит.
		lua_pushinteger(L, result);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int plane_goto_coords(lua_State* L) {// plane_goto_coords(car, x, y, z) - самолет летит на координаты.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			float x = lua_tonumber(L, 2); float y = lua_tonumber(L, 3); float z = lua_tonumber(L, 4);
			if (car && car->m_nVehicleClass == VEHICLE_PLANE) {
				CVector pos = { x, y, z };
				Command<COMMAND_PLANE_GOTO_COORDS>(CPools::GetVehicleRef(car), pos.x, pos.y, pos.z);// лететь.
			}
			return 0;
		}
		else { throw "bad argument in function plane_goto_coords"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int plane_is_player_in_any_plane(lua_State* L) {// игрок в любом самолете?.
	try {
		int result = 0;
		Command<COMMAND_IS_PLAYER_IN_ANY_PLANE>(&result);// игрок в самолете.
		lua_pushinteger(L, result);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int plane_is_char_in_any_plane(lua_State* L) {// пед в любом самолете?.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			int result = 0;
			Command<COMMAND_IS_CHAR_IN_ANY_PLANE>(CPools::GetPedRef(ped), &result);// пед в самолете.
			lua_pushinteger(L, result);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function plane_is_char_in_any_plane"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int find_drug_plane_coords(lua_State* L) {// координаты самолета с наркотиками.
	try {
		float x = 0.0f, y = 0.0f, z = 0.0f;
		Command<COMMAND_FIND_DRUG_PLANE_COORDINATES>(&x, &y, &z);// координаты.
		lua_pushnumber(L, x); lua_pushnumber(L, y); lua_pushnumber(L, z);// отправить в стек.
		return 3;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int find_drop_off_plane_coords(lua_State* L) {// координаты самолета-доставщика.
	try {
		float x = 0.0f, y = 0.0f, z = 0.0f;
		Command<COMMAND_FIND_DROP_OFF_PLANE_COORDINATES>(&x, &y, &z);// координаты.
		lua_pushnumber(L, x); lua_pushnumber(L, y); lua_pushnumber(L, z);// отправить в стек.
		return 3;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int has_drug_plane_shot_down(lua_State* L) {// самолет с наркотиками сбит?.
	try {
		int result = 0;
		Command<COMMAND_HAS_DRUG_PLANE_BEEN_SHOT_DOWN>(&result);// сбит.
		lua_pushinteger(L, result);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int has_drop_off_plane_shot_down(lua_State* L) {// самолет-доставщик сбит?.
	try {
		int result = 0;
		Command<COMMAND_HAS_DROP_OFF_PLANE_BEEN_SHOT_DOWN>(&result);// сбит.
		lua_pushinteger(L, result);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int boat_goto_coords(lua_State* L) {// boat_goto_coords(car, x, y, z) - катер плывет на координаты.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			float x = lua_tonumber(L, 2); float y = lua_tonumber(L, 3); float z = lua_tonumber(L, 4);
			if (car && car->m_nVehicleClass == VEHICLE_BOAT) {
				CVector pos = { x, y, z };
				Command<COMMAND_BOAT_GOTO_COORDS>(CPools::GetVehicleRef(car), pos.x, pos.y, pos.z);// плыть.
			}
			return 0;
		}
		else { throw "bad argument in function boat_goto_coords"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int boat_set_cruise_speed(lua_State* L) {// установить крейсерскую скорость катера.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			float speed = lua_tonumber(L, 2);// крейсерская скорость.
			if (car && car->m_nVehicleClass == VEHICLE_BOAT) {
				Command<COMMAND_SET_BOAT_CRUISE_SPEED>(CPools::GetVehicleRef(car), speed);// установить скорость.
			}
			return 0;
		}
		else { throw "bad argument in function boat_set_cruise_speed"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int boat_anchor(lua_State* L) {// boat_anchor(car, true/false) - якорить катер.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			bool anchor = lua_toboolean(L, 2);// якорь.
			if (car && car->m_nVehicleClass == VEHICLE_BOAT) {
				Command<COMMAND_ANCHOR_BOAT>(CPools::GetVehicleRef(car), anchor);// якорить.
			}
			return 0;
		}
		else { throw "bad argument in function boat_anchor"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int boat_start_foam_animation(lua_State* L) {// включить пену катера.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			if (car && car->m_nVehicleClass == VEHICLE_BOAT) {
				Command<COMMAND_START_BOAT_FOAM_ANIMATION>(CPools::GetVehicleRef(car));// пена.
			}
			return 0;
		}
		else { throw "bad argument in function boat_start_foam_animation"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int boat_update_foam_animation(lua_State* L) {// обновить пену катера.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			if (car && car->m_nVehicleClass == VEHICLE_BOAT) {
				Command<COMMAND_UPDATE_BOAT_FOAM_ANIMATION>(CPools::GetVehicleRef(car));// обновить пену.
			}
			return 0;
		}
		else { throw "bad argument in function boat_update_foam_animation"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int boat_is_player_in_any_boat(lua_State* L) {// игрок в любом катере?.
	try {
		int result = 0;
		Command<COMMAND_IS_PLAYER_IN_ANY_BOAT>(&result);// игрок в катере.
		lua_pushinteger(L, result);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int boat_is_char_in_any_boat(lua_State* L) {// пед в любом катере?.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			int result = 0;
			Command<COMMAND_IS_CHAR_IN_ANY_BOAT>(CPools::GetPedRef(ped), &result);// пед в катере.
			lua_pushinteger(L, result);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function boat_is_char_in_any_boat"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int is_vehicle_boat(lua_State* L) {// транспорт это катер?.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			int result = 0;
			Command<COMMAND_IS_BOAT>(CPools::GetVehicleRef(car), &result);// катер.
			lua_pushinteger(L, result);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function is_vehicle_boat"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int train_catch_char_obj(lua_State* L) {// train_catch_char_obj(ped, true/false) - пед ловит поезд.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			bool catch_train = lua_toboolean(L, 2);// поймать.
			Command<COMMAND_SET_CHAR_OBJ_CATCH_TRAIN>(CPools::GetPedRef(ped), catch_train);// ловить.
			return 0;
		}
		else { throw "bad argument in function train_catch_char_obj"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int train_catch_coll_obj(lua_State* L) {// train_catch_coll_obj(obj, true/false) - объект ловит поезд.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CObject* obj = findobjinpool(p);// получить указатель на объект.
			bool catch_train = lua_toboolean(L, 2);// поймать.
			Command<COMMAND_SET_COLL_OBJ_CATCH_TRAIN>(CPools::GetObjectRef(obj), catch_train);// ловить.
			return 0;
		}
		else { throw "bad argument in function train_catch_coll_obj"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int rc_give_remote_controlled_car_to_player(lua_State* L) {// выдать RC-машину игроку.
	try {
		int result = 0;
		Command<COMMAND_GIVE_REMOTE_CONTROLLED_CAR_TO_PLAYER>(&result);// выдать RC-машину.
		if (result != 0) {
			lua_pushinteger(L, result);// отправить в стек.
			return 1;
		}
		return 0;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int rc_give_remote_controlled_model_to_player(lua_State* L) {// выдать RC-модель игроку.
	try {
		int result = 0;
		Command<COMMAND_GIVE_REMOTE_CONTROLLED_MODEL_TO_PLAYER>(&result);// выдать RC-модель.
		if (result != 0) {
			lua_pushinteger(L, result);// отправить в стек.
			return 1;
		}
		return 0;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int rc_get_remote_controlled_car(lua_State* L) {// получить RC-машину.
	try {
		int result = 0;
		Command<COMMAND_GET_REMOTE_CONTROLLED_CAR>(&result);// получить RC-машину.
		if (result != 0) {
			lua_pushinteger(L, result);// отправить в стек.
			return 1;
		}
		return 0;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int rc_buggy_blow_up(lua_State* L) {// взорвать RC-багги.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			Command<COMMAND_BLOW_UP_RC_BUGGY>(CPools::GetVehicleRef(car));// взорвать.
			return 0;
		}
		else { throw "bad argument in function rc_buggy_blow_up"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int rc_buggy_remove(lua_State* L) {// удалить RC-багги.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			Command<COMMAND_REMOVE_RC_BUGGY>(CPools::GetVehicleRef(car));// удалить.
			return 0;
		}
		else { throw "bad argument in function rc_buggy_remove"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int rc_detonate_enable(lua_State* L) {// rc_detonate_enable(car, true/false) - включить детонацию RC.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			bool enable = lua_toboolean(L, 2);// включить.
			Command<COMMAND_SET_ENABLE_RC_DETONATE>(CPools::GetVehicleRef(car), enable);// детонация.
			return 0;
		}
		else { throw "bad argument in function rc_detonate_enable"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int rc_detonate_on_contact_enable(lua_State* L) {// rc_detonate_on_contact_enable(car, true/false) - детонация RC при контакте.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			bool enable = lua_toboolean(L, 2);// включить.
			Command<COMMAND_SET_ENABLE_RC_DETONATE_ON_CONTACT>(CPools::GetVehicleRef(car), enable);// детонация при контакте.
			return 0;
		}
		else { throw "bad argument in function rc_detonate_on_contact_enable"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int player_is_in_remote_mode(lua_State* L) {// игрок в режиме пульта?.
	try {
		int result = 0;
		Command<COMMAND_IS_PLAYER_IN_REMOTE_MODE>(&result);// режим пульта.
		lua_pushinteger(L, result);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int set_fade_jumpcut_after_rc_explosion(lua_State* L) {// set_fade_jumpcut_after_rc_explosion(true/false) - затухание после взрыва RC.
	try {
		if (LUA_TBOOLEAN == lua_type(L, 1)) {
			bool fade = lua_toboolean(L, 1);// затухание.
			Command<COMMAND_SET_FADE_AND_JUMPCUT_AFTER_RC_EXPLOSION>(fade);// установить.
			return 0;
		}
		else { throw "bad argument in function set_fade_jumpcut_after_rc_explosion"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int bike_is_player_on_any_bike(lua_State* L) {// игрок на любом мотоцикле?.
	try {
		int result = 0;
		Command<COMMAND_IS_PLAYER_ON_ANY_BIKE>(&result);// игрок на мотоцикле.
		lua_pushinteger(L, result);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int bike_is_char_on_any_bike(lua_State* L) {// пед на любом мотоцикле?.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			int result = 0;
			Command<COMMAND_IS_CHAR_ON_ANY_BIKE>(CPools::GetPedRef(ped), &result);// пед на мотоцикле.
			lua_pushinteger(L, result);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function bike_is_char_on_any_bike"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int is_char_in_flying_vehicle(lua_State* L) {// пед в летающем транспорте?.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			int result = 0;
			Command<COMMAND_IS_CHAR_IN_FLYING_VEHICLE>(CPools::GetPedRef(ped), &result);// пед в летающем.
			lua_pushinteger(L, result);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function is_char_in_flying_vehicle"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int is_player_in_flying_vehicle(lua_State* L) {// игрок в летающем транспорте?.
	try {
		int result = 0;
		Command<COMMAND_IS_PLAYER_IN_FLYING_VEHICLE>(&result);// игрок в летающем.
		lua_pushinteger(L, result);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int crane_activate(lua_State* L) {// активировать кран.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			Command<COMMAND_ACTIVATE_CRANE>(CPools::GetVehicleRef(car));// активировать кран.
			return 0;
		}
		else { throw "bad argument in function crane_activate"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int crane_is_carrying_car(lua_State* L) {// кран несет авто?.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			bool carrying = CCranes::IsThisCarBeingCarriedByAnyCrane(car);// несет.
			lua_pushinteger(L, carrying ? 1 : 0);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function crane_is_carrying_car"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int crane_is_targetting_car(lua_State* L) {// кран целится в авто?.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			bool targetted = CCranes::IsThisCarBeingTargettedByAnyCrane(car);// целится.
			lua_pushinteger(L, targetted ? 1 : 0);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function crane_is_targetting_car"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int escalators_count(lua_State* L) {// количество эскалаторов.
	try {
		int count = CEscalators::NumEscalators;// кол-во эскалаторов.
		lua_pushinteger(L, count);// отправить в стек.
		return 1;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int zone_find_index(lua_State* L) {// zone_find_index(name) - найти индекс зоны по имени.
	try {
		if (LUA_TSTRING == lua_type(L, 1)) {
			const char* name = lua_tostring(L, 1);// имя зоны.
			int index = CTheZones::FindZoneByLabelAndReturnIndex((char*)name, ZONE_DEFAULT);// найти.
			lua_pushinteger(L, index);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function zone_find_index"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int zone_get_level_from_position(lua_State* L) {// zone_get_level_from_position(x, y, z) - уровень зоны по позиции.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {
			float x = lua_tonumber(L, 1); float y = lua_tonumber(L, 2); float z = lua_tonumber(L, 3);
			CVector pos = { x, y, z };
			eLevelName level = CTheZones::GetLevelFromPosition(&pos);// уровень.
			lua_pushinteger(L, level);// отправить в стек.
			return 1;
		}
		else { throw "bad argument in function zone_get_level_from_position"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};
// ================= ЭТАП 12: Остальной транспорт GTA VC =================

// ==================== КРАНЫ ====================

int crusher_crane_activate(lua_State* L) {// () активировать пресс-кран.
	try {
		Command<COMMAND_ACTIVATE_CRUSHER_CRANE>();
		return 0;
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int military_crane_activate(lua_State* L) {// () активировать военный кран.
	try {
		Command<COMMAND_ACTIVATE_MILITARY_CRANE>();
		return 0;
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int crane_deactivate(lua_State* L) {// (car) деактивировать кран.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			Command<COMMAND_DEACTIVATE_CRANE>(car);
			return 0;
		}
		else { throw "bad argument in function crane_deactivate"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int crane_military_collected_all(lua_State* L) {// () военный кран собрал все авто.
	try {
		int result = 0;
		Command<COMMAND_HAS_MILITARY_CRANE_COLLECTED_ALL_CARS>(&result);
		lua_pushinteger(L, result);
		return 1;
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int crane_is_lifting_car(lua_State* L) {// (car) кран поднимает авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			int result = 0;
			Command<COMMAND_IS_CRANE_LIFTING_CAR>(car, &result);
			lua_pushinteger(L, result);
			return 1;
		}
		else { throw "bad argument in function crane_is_lifting_car"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

// ==================== КАТЕРА ====================

int boat_stop(lua_State* L) {// (car) остановить катер.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			Command<COMMAND_BOAT_STOP>(car);
			return 0;
		}
		else { throw "bad argument in function boat_stop"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

// ==================== ВЕРТОЛЕТЫ ====================

int heli_goto_coords(lua_State* L) {// (car, x, y, z) вертолет на координаты.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			float x = lua_tonumber(L, 2);
			float y = lua_tonumber(L, 3);
			float z = lua_tonumber(L, 4);
			Command<COMMAND_HELI_GOTO_COORDS>(car, x, y, z);
			return 0;
		}
		else { throw "bad argument in function heli_goto_coords"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int heli_create_dust_effect(lua_State* L) {// (car) пыль катсценного вертолета.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			Command<COMMAND_CREATE_DUST_EFFECT_FOR_CUTSCENE_HELI>(car);
			return 0;
		}
		else { throw "bad argument in function heli_create_dust_effect"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

// ==================== УПРАВЛЕНИЕ АВТО ====================

int car_goto_accurate(lua_State* L) {// (car, x, y, z) точная езда на координаты.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			float x = lua_tonumber(L, 2);
			float y = lua_tonumber(L, 3);
			float z = lua_tonumber(L, 4);
			Command<COMMAND_CAR_GOTO_COORDINATES_ACCURATE>(car, x, y, z);
			return 0;
		}
		else { throw "bad argument in function car_goto_accurate"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int car_set_idle(lua_State* L) {// (car) авто в ожидание.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			Command<COMMAND_CAR_SET_IDLE>(car);
			return 0;
		}
		else { throw "bad argument in function car_set_idle"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int car_wander_randomly(lua_State* L) {// (car) авто едет случайно.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			Command<COMMAND_CAR_WANDER_RANDOMLY>(car);
			return 0;
		}
		else { throw "bad argument in function car_wander_randomly"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int car_face_coord(lua_State* L) {// (car, x, y, z) повернуть к координатам.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			float x = lua_tonumber(L, 2);
			float y = lua_tonumber(L, 3);
			float z = lua_tonumber(L, 4);
			Command<COMMAND_TURN_CAR_TO_FACE_COORD>(car, x, y, z);
			return 0;
		}
		else { throw "bad argument in function car_face_coord"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int freeze_car_position(lua_State* L) {// (car, bool) заморозить позицию.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			bool flag = lua_toboolean(L, 2);
			Command<COMMAND_FREEZE_CAR_POSITION>(car, flag);
			return 0;
		}
		else { throw "bad argument in function freeze_car_position"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int freeze_car_no_collide(lua_State* L) {// (car, bool) заморозить без коллизий.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			bool flag = lua_toboolean(L, 2);
			Command<COMMAND_FREEZE_CAR_POSITION_AND_DONT_LOAD_COLLISION>(car, flag);
			return 0;
		}
		else { throw "bad argument in function freeze_car_no_collide"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int set_car_status(lua_State* L) {// (car, int) статус авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			int status = lua_tointeger(L, 2);
			Command<COMMAND_SET_CAR_STATUS>(car, status);
			return 0;
		}
		else { throw "bad argument in function set_car_status"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int set_car_mission(lua_State* L) {// (car, int) миссия авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			int mission = lua_tointeger(L, 2);
			Command<COMMAND_SET_CAR_MISSION>(car, mission);
			return 0;
		}
		else { throw "bad argument in function set_car_mission"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int car_block_car(lua_State* L) {// (car1, car2) блокировать авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {
			const void* p1 = lua_topointer(L, 1);
			CVehicle* car1 = findcarinpool(p1);
			const void* p2 = lua_topointer(L, 2);
			CVehicle* car2 = findcarinpool(p2);
			Command<COMMAND_SET_CAR_BLOCK_CAR>(car1, car2);
			return 0;
		}
		else { throw "bad argument in function car_block_car"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int car_ram_car(lua_State* L) {// (car1, car2) таранить.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {
			const void* p1 = lua_topointer(L, 1);
			CVehicle* car1 = findcarinpool(p1);
			const void* p2 = lua_topointer(L, 2);
			CVehicle* car2 = findcarinpool(p2);
			Command<COMMAND_SET_CAR_RAM_CAR>(car1, car2);
			return 0;
		}
		else { throw "bad argument in function car_ram_car"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int car_change_lane(lua_State* L) {// (car, bool) смена полосы.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			bool flag = lua_toboolean(L, 2);
			Command<COMMAND_SET_CAR_CHANGE_LANE>(car, flag);
			return 0;
		}
		else { throw "bad argument in function car_change_lane"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int car_drive_straight(lua_State* L) {// (car, bool) ехать прямо.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			bool flag = lua_toboolean(L, 2);
			Command<COMMAND_SET_CAR_DRIVE_STRAIGHT_AHEAD>(car, flag);
			return 0;
		}
		else { throw "bad argument in function car_drive_straight"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int set_car_forward_speed(lua_State* L) {// (car, float) скорость вперед.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			float speed = lua_tonumber(L, 2);
			Command<COMMAND_SET_CAR_FORWARD_SPEED>(car, speed);
			return 0;
		}
		else { throw "bad argument in function set_car_forward_speed"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int car_wait(lua_State* L) {// (car, int) приказать ждать ms.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			int time = lua_tointeger(L, 2);
			Command<COMMAND_SET_CAR_WAIT>(car, time);
			return 0;
		}
		else { throw "bad argument in function car_wait"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int car_route_seed(lua_State* L) {// (car, int) seed маршрута.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			int seed = lua_tointeger(L, 2);
			Command<COMMAND_SET_CAR_RANDOM_ROUTE_SEED>(car, seed);
			return 0;
		}
		else { throw "bad argument in function car_route_seed"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int car_straight_dist(lua_State* L) {// (car, int) дистанция прямой линии.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			int dist = lua_tointeger(L, 2);
			Command<COMMAND_SET_CAR_STRAIGHT_LINE_DISTANCE>(car, dist);
			return 0;
		}
		else { throw "bad argument in function car_straight_dist"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int car_convoy(lua_State* L) {// (car, bool) конвойное авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			bool flag = lua_toboolean(L, 2);
			Command<COMMAND_MARK_CAR_AS_CONVOY_CAR>(car, flag);
			return 0;
		}
		else { throw "bad argument in function car_convoy"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int car_make_stronger(lua_State* L) {// (car) усилить авто Крейга.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			Command<COMMAND_MAKE_CRAIGS_CAR_A_BIT_STRONGER>(car);
			return 0;
		}
		else { throw "bad argument in function car_make_stronger"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int car_to_player(lua_State* L) {// (car) авто к игроку.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			Command<COMMAND_SET_JAMES_CAR_ON_PATH_TO_PLAYER>(car);
			return 0;
		}
		else { throw "bad argument in function car_to_player"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int car_fade_in(lua_State* L) {// (car, float) затухание.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			float time = lua_tonumber(L, 2);
			Command<COMMAND_SET_VEHICLE_TO_FADE_IN>(car, time);
			return 0;
		}
		else { throw "bad argument in function car_fade_in"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int car_on_trailer(lua_State* L) {// (car) на прицеп.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			Command<COMMAND_PUT_CAR_ON_TRAILER>(car);
			return 0;
		}
		else { throw "bad argument in function car_on_trailer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

// ==================== СВОЙСТВА АВТО ====================

int set_car_visible(lua_State* L) {// (car, bool) видимость.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			bool flag = lua_toboolean(L, 2);
			Command<COMMAND_SET_CAR_VISIBLE>(car, flag);
			return 0;
		}
		else { throw "bad argument in function set_car_visible"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int set_car_watertight(lua_State* L) {// (car, bool) водонепроницаемость.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			bool flag = lua_toboolean(L, 2);
			Command<COMMAND_SET_CAR_WATERTIGHT>(car, flag);
			return 0;
		}
		else { throw "bad argument in function set_car_watertight"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int set_car_big_wheels(lua_State* L) {// (car, bool) большие колеса.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			bool flag = lua_toboolean(L, 2);
			Command<COMMAND_SET_CAR_BIG_WHEELS>(car, flag);
			return 0;
		}
		else { throw "bad argument in function set_car_big_wheels"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int set_car_bproof(lua_State* L) {// (car, bool) пуленепробиваемость.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			bool flag = lua_toboolean(L, 2);
			Command<COMMAND_SET_CAR_BULLETPROOF>(car, flag);
			return 0;
		}
		else { throw "bad argument in function set_car_bproof"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int set_car_fproof(lua_State* L) {// (car, bool) огнеупорность.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			bool flag = lua_toboolean(L, 2);
			Command<COMMAND_SET_CAR_FLAMEPROOF>(car, flag);
			return 0;
		}
		else { throw "bad argument in function set_car_fproof"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int set_car_rproof(lua_State* L) {// (car, bool) ракетоупорность.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			bool flag = lua_toboolean(L, 2);
			Command<COMMAND_SET_CAR_ROCKETPROOF>(car, flag);
			return 0;
		}
		else { throw "bad argument in function set_car_rproof"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int set_car_no_collide(lua_State* L) {// (car, bool) без коллизий.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			bool flag = lua_toboolean(L, 2);
			Command<COMMAND_SET_CAR_NO_COLLIDE>(car, flag);
			return 0;
		}
		else { throw "bad argument in function set_car_no_collide"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int set_car_burst_tires(lua_State* L) {// (car, bool) пробиваемость шин.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			bool flag = lua_toboolean(L, 2);
			Command<COMMAND_SET_CAN_BURST_CAR_TYRES>(car, flag);
			return 0;
		}
		else { throw "bad argument in function set_car_burst_tires"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int set_car_dmg_player_only(lua_State* L) {// (car, bool) урон только от игрока.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			bool flag = lua_toboolean(L, 2);
			Command<COMMAND_SET_CAR_ONLY_DAMAGED_BY_PLAYER>(car, flag);
			return 0;
		}
		else { throw "bad argument in function set_car_dmg_player_only"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int car_load_collision(lua_State* L) {// (car, bool) загрузка коллизий.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			bool flag = lua_toboolean(L, 2);
			Command<COMMAND_SET_LOAD_COLLISION_FOR_CAR_FLAG>(car, flag);
			return 0;
		}
		else { throw "bad argument in function car_load_collision"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int car_upside_safe(lua_State* L) {// (car, bool) перевернутое не ломается.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			bool flag = lua_toboolean(L, 2);
			Command<COMMAND_SET_UPSIDEDOWN_CAR_NOT_DAMAGED>(car, flag);
			return 0;
		}
		else { throw "bad argument in function car_upside_safe"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int car_change_color(lua_State* L) {// (car, int, int) сменить цвет.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			int color1 = lua_tointeger(L, 2);
			int color2 = lua_tointeger(L, 3);
			Command<COMMAND_CHANGE_CAR_COLOUR>(car, color1, color2);
			return 0;
		}
		else { throw "bad argument in function car_change_color"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int car_set_comp(lua_State* L) {// (car, int, int) компоненты модели.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			int comp = lua_tointeger(L, 2);
			int val = lua_tointeger(L, 3);
			Command<COMMAND_SET_CAR_MODEL_COMPONENTS>(car, comp, val);
			return 0;
		}
		else { throw "bad argument in function car_set_comp"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

// ==================== ЗДОРОВЬЕ И УГОЛ ====================

int set_car_health(lua_State* L) {// (car, float) здоровье.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			float health = lua_tonumber(L, 2);
			Command<COMMAND_SET_CAR_HEALTH>(car, health);
			return 0;
		}
		else { throw "bad argument in function set_car_health"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int get_car_health(lua_State* L) {// (car) получить здоровье.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			float result = 0.0f;
			Command<COMMAND_GET_CAR_HEALTH>(car, &result);
			lua_pushnumber(L, result);
			return 1;
		}
		else { throw "bad argument in function get_car_health"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int get_car_heading(lua_State* L) {// (car) получить угол.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			float result = 0.0f;
			Command<COMMAND_GET_CAR_HEADING>(car, &result);
			lua_pushnumber(L, result);
			return 1;
		}
		else { throw "bad argument in function get_car_heading"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

// ==================== ВЕКТОРА ====================

int get_car_forward_x(lua_State* L) {// (car) вектор X.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			float result = 0.0f;
			Command<COMMAND_GET_CAR_FORWARD_X>(car, &result);
			lua_pushnumber(L, result);
			return 1;
		}
		else { throw "bad argument in function get_car_forward_x"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int get_car_forward_y(lua_State* L) {// (car) вектор Y.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			float result = 0.0f;
			Command<COMMAND_GET_CAR_FORWARD_Y>(car, &result);
			lua_pushnumber(L, result);
			return 1;
		}
		else { throw "bad argument in function get_car_forward_y"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

// ==================== ПРОВЕРКИ АВТО (boolean) ====================

int is_car_in_air(lua_State* L) {// (car) в воздухе?
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			int result = 0;
			Command<COMMAND_IS_CAR_IN_AIR>(car, &result);
			lua_pushinteger(L, result);
			return 1;
		}
		else { throw "bad argument in function is_car_in_air"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int is_car_in_air_proper(lua_State* L) {// (car) в воздухе точно?
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			int result = 0;
			Command<COMMAND_IS_CAR_IN_AIR_PROPER>(car, &result);
			lua_pushinteger(L, result);
			return 1;
		}
		else { throw "bad argument in function is_car_in_air_proper"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int car_health_greater(lua_State* L) {// (car, float) здоровье больше?
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			float health = lua_tonumber(L, 2);
			int result = 0;
			Command<COMMAND_IS_CAR_HEALTH_GREATER>(car, health, &result);
			lua_pushinteger(L, result);
			return 1;
		}
		else { throw "bad argument in function car_health_greater"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int is_car_crushed(lua_State* L) {// (car) прессовано?
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			int result = 0;
			Command<COMMAND_IS_CAR_CRUSHED>(car, &result);
			lua_pushinteger(L, result);
			return 1;
		}
		else { throw "bad argument in function is_car_crushed"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int is_car_remap(lua_State* L) {// (car) перекрашено?
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			int result = 0;
			Command<COMMAND_IS_CAR_REMAP>(car, &result);
			lua_pushinteger(L, result);
			return 1;
		}
		else { throw "bad argument in function is_car_remap"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int is_car_stuck_roof(lua_State* L) {// (car) на крыше?
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			int result = 0;
			Command<COMMAND_IS_CAR_STUCK_ON_ROOF>(car, &result);
			lua_pushinteger(L, result);
			return 1;
		}
		else { throw "bad argument in function is_car_stuck_roof"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int car_is_tire_burst(lua_State* L) {// (car) шина пробита?
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			int result = 0;
			Command<COMMAND_IS_CAR_TYRE_BURST>(car, &result);
			lua_pushinteger(L, result);
			return 1;
		}
		else { throw "bad argument in function car_is_tire_burst"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int is_car_visibly_damaged(lua_State* L) {// (car) видимо повреждено?
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			int result = 0;
			Command<COMMAND_IS_CAR_VISIBLY_DAMAGED>(car, &result);
			lua_pushinteger(L, result);
			return 1;
		}
		else { throw "bad argument in function is_car_visibly_damaged"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int car_waiting_collision(lua_State* L) {// (car) ждет коллизии?
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			int result = 0;
			Command<COMMAND_IS_CAR_WAITING_FOR_WORLD_COLLISION>(car, &result);
			lua_pushinteger(L, result);
			return 1;
		}
		else { throw "bad argument in function car_waiting_collision"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int is_car_on_trailer(lua_State* L) {// (car) на прицепе?
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			int result = 0;
			Command<COMMAND_IS_CAR_ON_TRAILER>(car, &result);
			lua_pushinteger(L, result);
			return 1;
		}
		else { throw "bad argument in function is_car_on_trailer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int car_damaged_by_car(lua_State* L) {// (car1, car2) повреждено авто?
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {
			const void* p1 = lua_topointer(L, 1);
			CVehicle* car1 = findcarinpool(p1);
			const void* p2 = lua_topointer(L, 2);
			CVehicle* car2 = findcarinpool(p2);
			int result = 0;
			Command<COMMAND_HAS_CAR_BEEN_DAMAGED_BY_CAR>(car1, car2, &result);
			lua_pushinteger(L, result);
			return 1;
		}
		else { throw "bad argument in function car_damaged_by_car"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int car_damaged_by_ped(lua_State* L) {// (car, ped) повреждено педом?
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {
			const void* p1 = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p1);
			const void* p2 = lua_topointer(L, 2);
			CPed* ped = findpedinpool(p2);
			int result = 0;
			Command<COMMAND_HAS_CAR_BEEN_DAMAGED_BY_CHAR>(car, ped, &result);
			lua_pushinteger(L, result);
			return 1;
		}
		else { throw "bad argument in function car_damaged_by_ped"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int car_just_sunk(lua_State* L) {// (car) утонуло?
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			int result = 0;
			Command<COMMAND_HAS_CAR_JUST_SUNK>(car, &result);
			lua_pushinteger(L, result);
			return 1;
		}
		else { throw "bad argument in function car_just_sunk"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int car_drowning(lua_State* L) {// (car) тонет?
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			int result = 0;
			Command<COMMAND_IS_CAR_DROWNING_IN_WATER>(car, &result);
			lua_pushinteger(L, result);
			return 1;
		}
		else { throw "bad argument in function car_drowning"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int car_has_weapon(lua_State* L) {// (car, int) оружие на борту?
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			int weapon = lua_tointeger(L, 2);
			int result = 0;
			Command<COMMAND_HAS_CAR_GOT_WEAPON>(car, weapon, &result);
			lua_pushinteger(L, result);
			return 1;
		}
		else { throw "bad argument in function car_has_weapon"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int car_any_bomb(lua_State* L) {// (car) есть бомба?
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			int result = 0;
			Command<COMMAND_IS_CAR_ARMED_WITH_ANY_BOMB>(car, &result);
			lua_pushinteger(L, result);
			return 1;
		}
		else { throw "bad argument in function car_any_bomb"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int car_has_bomb(lua_State* L) {// (car, int) тип бомбы?
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			int bomb = lua_tointeger(L, 2);
			int result = 0;
			Command<COMMAND_IS_CAR_ARMED_WITH_BOMB>(car, bomb, &result);
			lua_pushinteger(L, result);
			return 1;
		}
		else { throw "bad argument in function car_has_bomb"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int is_car_in_water(lua_State* L) {// (car) в воде?
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			int result = 0;
			Command<COMMAND_IS_CAR_IN_WATER>(car, &result);
			lua_pushinteger(L, result);
			return 1;
		}
		else { throw "bad argument in function is_car_in_water"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int fire_button_pressed(lua_State* L) {// () кнопка огня?
	try {
		int result = 0;
		Command<COMMAND_IS_IN_CAR_FIRE_BUTTON_PRESSED>(&result);
		lua_pushinteger(L, result);
		return 1;
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int is_car_alive(lua_State* L) {// (car) живо?
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			int result = 0;
			Command<COMMAND_IS_CAR_STILL_ALIVE>(car, &result);
			lua_pushinteger(L, result);
			return 1;
		}
		else { throw "bad argument in function is_car_alive"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int does_vehicle_exist(lua_State* L) {// (car) существует?
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			int result = 0;
			Command<COMMAND_DOES_VEHICLE_EXIST>(car, &result);
			lua_pushinteger(L, result);
			return 1;
		}
		else { throw "bad argument in function does_vehicle_exist"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int is_car_in_garage(lua_State* L) {// (car) в гараже?
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			int result = 0;
			Command<COMMAND_IS_CAR_IN_MISSION_GARAGE>(car, &result);
			lua_pushinteger(L, result);
			return 1;
		}
		else { throw "bad argument in function is_car_in_garage"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int was_ever_police(lua_State* L) {// (car) был полицейским?
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			int result = 0;
			Command<COMMAND_WAS_VEHICLE_EVER_POLICE>(car, &result);
			lua_pushinteger(L, result);
			return 1;
		}
		else { throw "bad argument in function was_ever_police"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

// ==================== ОЧИСТКА ====================

int clear_car_dmg(lua_State* L) {// (car) очистить урон.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			Command<COMMAND_CLEAR_CAR_LAST_DAMAGE_ENTITY>(car);
			return 0;
		}
		else { throw "bad argument in function clear_car_dmg"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int clear_car_wep_dmg(lua_State* L) {// (car) очистить урон оружием.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			Command<COMMAND_CLEAR_CAR_LAST_WEAPON_DAMAGE>(car);
			return 0;
		}
		else { throw "bad argument in function clear_car_wep_dmg"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int remove_stuck_check(lua_State* L) {// (car) убрать проверку застревания.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			Command<COMMAND_REMOVE_STUCK_CAR_CHECK>(car);
			return 0;
		}
		else { throw "bad argument in function remove_stuck_check"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int remove_upside_check(lua_State* L) {// (car) убрать проверку переворота.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			Command<COMMAND_REMOVE_UPSIDEDOWN_CAR_CHECK>(car);
			return 0;
		}
		else { throw "bad argument in function remove_upside_check"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int add_upside_check(lua_State* L) {// (car) добавить проверку переворота.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			Command<COMMAND_ADD_UPSIDEDOWN_CAR_CHECK>(car);
			return 0;
		}
		else { throw "bad argument in function add_upside_check"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int unlock_doors_area(lua_State* L) {// (car) открыть двери.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			Command<COMMAND_UNLOCK_ALL_CAR_DOORS_IN_AREA>(car);
			return 0;
		}
		else { throw "bad argument in function unlock_doors_area"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

// ==================== ДВЕРИ/БАГАЖНИК ====================

int close_all_doors(lua_State* L) {// (car) закрыть все двери.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			Command<COMMAND_CLOSE_ALL_CAR_DOORS>(car);
			return 0;
		}
		else { throw "bad argument in function close_all_doors"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int close_car_boot(lua_State* L) {// (car) закрыть багажник.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			Command<COMMAND_CLOSE_CAR_BOOT>(car);
			return 0;
		}
		else { throw "bad argument in function close_car_boot"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int pop_car_boot(lua_State* L) {// (car) открыть багажник.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			Command<COMMAND_POP_CAR_BOOT>(car);
			return 0;
		}
		else { throw "bad argument in function pop_car_boot"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int pop_car_boot_physics(lua_State* L) {// (car) открыть физикой.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			Command<COMMAND_POP_CAR_BOOT_USING_PHYSICS>(car);
			return 0;
		}
		else { throw "bad argument in function pop_car_boot_physics"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int lock_doors(lua_State* L) {// (car, bool) запереть двери.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			bool flag = lua_toboolean(L, 2);
			Command<COMMAND_LOCK_CAR_DOORS>(car, flag);
			return 0;
		}
		else { throw "bad argument in function lock_doors"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int change_car_lock(lua_State* L) {// (car, int) сменить замок.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			int lock = lua_tointeger(L, 2);
			Command<COMMAND_CHANGE_CAR_LOCK>(car, lock);
			return 0;
		}
		else { throw "bad argument in function change_car_lock"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

// ==================== ПЕД/ВОДИТЕЛЬ ====================

int get_car_driver_ref(lua_State* L) {// (car) водитель.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			CPed* ped = nullptr;
			Command<COMMAND_GET_DRIVER_OF_CAR>(car, &ped);
			if (ped) {
				lua_pushlightuserdata(L, (void*)CPools::GetPedRef(ped));
			}
			else {
				lua_pushnil(L);
			}
			return 1;
		}
		else { throw "bad argument in function get_car_driver_ref"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int get_char_in_seat(lua_State* L) {// (car, int seat) пассажир.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			int seat = lua_tointeger(L, 2);
			CPed* ped = nullptr;
			Command<COMMAND_GET_CHAR_IN_CAR_PASSENGER_SEAT>(car, seat, &ped);
			if (ped) {
				lua_pushlightuserdata(L, (void*)CPools::GetPedRef(ped));
			}
			else {
				lua_pushnil(L);
			}
			return 1;
		}
		else { throw "bad argument in function get_char_in_seat"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int order_driver_out(lua_State* L) {// (ped) выгнать водителя.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);
			Command<COMMAND_ORDER_DRIVER_OUT_OF_CAR>(ped);
			return 0;
		}
		else { throw "bad argument in function order_driver_out"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int set_ped_max_drive_speed(lua_State* L) {// (ped, float) макс скорость.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);
			float speed = lua_tonumber(L, 2);
			Command<COMMAND_SET_CHAR_MAX_DRIVESPEED>(ped, speed);
			return 0;
		}
		else { throw "bad argument in function set_ped_max_drive_speed"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int set_ped_drive_aggression(lua_State* L) {// (ped, float) агрессия.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);
			float aggr = lua_tonumber(L, 2);
			Command<COMMAND_SET_CHAR_DRIVE_AGGRESSION>(ped, aggr);
			return 0;
		}
		else { throw "bad argument in function set_ped_drive_aggression"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int shuffle_to_driver(lua_State* L) {// (ped) пересесть за руль.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);
			Command<COMMAND_SET_CHAR_SHUFFLE_INTO_DRIVERS_SEAT>(ped);
			return 0;
		}
		else { throw "bad argument in function shuffle_to_driver"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int stay_when_jacked(lua_State* L) {// (ped, bool) не покидать при угоне.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);
			bool flag = lua_toboolean(L, 2);
			Command<COMMAND_SET_CHAR_STAY_IN_CAR_WHEN_JACKED>(ped, flag);
			return 0;
		}
		else { throw "bad argument in function stay_when_jacked"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int is_ped_in_police_car(lua_State* L) {// (ped) в полицейском?
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);
			int result = 0;
			Command<COMMAND_IS_CHAR_IN_ANY_POLICE_VEHICLE>(ped, &result);
			lua_pushinteger(L, result);
			return 1;
		}
		else { throw "bad argument in function is_ped_in_police_car"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int is_ped_touching_vehicle(lua_State* L) {// (ped) касается?
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);
			int result = 0;
			Command<COMMAND_IS_CHAR_TOUCHING_VEHICLE>(ped, &result);
			lua_pushinteger(L, result);
			return 1;
		}
		else { throw "bad argument in function is_ped_touching_vehicle"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int is_ped_leaving_to_die(lua_State* L) {// (ped) умирает из авто?
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);
			int result = 0;
			Command<COMMAND_IS_CHAR_LEAVING_VEHICLE_TO_DIE>(ped, &result);
			lua_pushinteger(L, result);
			return 1;
		}
		else { throw "bad argument in function is_ped_leaving_to_die"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int is_ped_drowning(lua_State* L) {// (ped) тонет?
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);
			int result = 0;
			Command<COMMAND_IS_CHAR_DROWNING_IN_WATER>(ped, &result);
			lua_pushinteger(L, result);
			return 1;
		}
		else { throw "bad argument in function is_ped_drowning"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int is_ped_in_water(lua_State* L) {// (ped) в воде?
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);
			int result = 0;
			Command<COMMAND_IS_CHAR_IN_WATER>(ped, &result);
			lua_pushinteger(L, result);
			return 1;
		}
		else { throw "bad argument in function is_ped_in_water"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int is_ped_in_zone(lua_State* L) {// (ped, string) в зоне?
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TSTRING == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);
			const char* zone = lua_tostring(L, 2);
			int result = 0;
			Command<COMMAND_IS_CHAR_IN_ZONE>(ped, zone, &result);
			lua_pushinteger(L, result);
			return 1;
		}
		else { throw "bad argument in function is_ped_in_zone"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int is_player_in_zone(lua_State* L) {// (string) игрок в зоне?
	try {
		if (LUA_TSTRING == lua_type(L, 1)) {
			const char* zone = lua_tostring(L, 1);
			int result = 0;
			Command<COMMAND_IS_PLAYER_IN_ZONE>(CWorld::PlayerInFocus, zone, &result);
			lua_pushinteger(L, result);
			return 1;
		}
		else { throw "bad argument in function is_player_in_zone"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

// ==================== ТЕЛЕПОРТАЦИЯ ====================

int warp_ped_from_car(lua_State* L) {// (ped, x, y, z) телепорт из авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);
			float x = lua_tonumber(L, 2);
			float y = lua_tonumber(L, 3);
			float z = lua_tonumber(L, 4);
			Command<COMMAND_WARP_CHAR_FROM_CAR_TO_COORD>(ped, x, y, z);
			return 0;
		}
		else { throw "bad argument in function warp_ped_from_car"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int warp_ped_passenger(lua_State* L) {// (ped, car, int) в пассажиры.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {
			const void* p1 = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p1);
			const void* p2 = lua_topointer(L, 2);
			CVehicle* car = findcarinpool(p2);
			int seat = lua_tointeger(L, 3);
			Command<COMMAND_WARP_CHAR_INTO_CAR_AS_PASSENGER>(ped, car, seat);
			return 0;
		}
		else { throw "bad argument in function warp_ped_passenger"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int warp_player_from_car(lua_State* L) {// (x, y, z) телепорт игрока.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {
			float x = lua_tonumber(L, 1);
			float y = lua_tonumber(L, 2);
			float z = lua_tonumber(L, 3);
			Command<COMMAND_WARP_PLAYER_FROM_CAR_TO_COORD>(CWorld::PlayerInFocus, x, y, z);
			return 0;
		}
		else { throw "bad argument in function warp_player_from_car"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

// ==================== ОРУЖИЕ ТРАНСПОРТА ====================

int set_car_ammo(lua_State* L) {// (car, int, int) боезапас.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			int weapon = lua_tointeger(L, 2);
			int ammo = lua_tointeger(L, 3);
			Command<COMMAND_SET_CAR_AMMO>(car, weapon, ammo);
			return 0;
		}
		else { throw "bad argument in function set_car_ammo"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int switch_car_weapons(lua_State* L) {// (car, bool) переключить оружие.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			bool flag = lua_toboolean(L, 2);
			Command<COMMAND_SWITCH_VEHICLE_WEAPONS>(car, flag);
			return 0;
		}
		else { throw "bad argument in function switch_car_weapons"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int set_car_weapon(lua_State* L) {// (car, int) текущее оружие.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			int weapon = lua_tointeger(L, 2);
			Command<COMMAND_SET_CURRENT_CAR_WEAPON>(car, weapon);
			return 0;
		}
		else { throw "bad argument in function set_car_weapon"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int fire_hunter_gun(lua_State* L) {// (car) выстрел Hunter.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			Command<COMMAND_FIRE_HUNTER_GUN>(car);
			return 0;
		}
		else { throw "bad argument in function fire_hunter_gun"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int set_tank_detonate(lua_State* L) {// (car, bool) танк взрывает авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			bool flag = lua_toboolean(L, 2);
			Command<COMMAND_SET_TANK_DETONATE_CARS>(car, flag);
			return 0;
		}
		else { throw "bad argument in function set_tank_detonate"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int ensure_drive_by(lua_State* L) {// () дать drive-by.
	try {
		Command<COMMAND_ENSURE_PLAYER_HAS_DRIVE_BY_WEAPON>();
		return 0;
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

// ==================== БОМБЫ/СИГНАЛИЗАЦИЯ ====================

int arm_car_bomb(lua_State* L) {// (car, int) установить бомбу.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			int bomb = lua_tointeger(L, 2);
			Command<COMMAND_ARM_CAR_WITH_BOMB>(car, bomb);
			return 0;
		}
		else { throw "bad argument in function arm_car_bomb"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int disarm_car_bomb(lua_State* L) {// (car) обезвредить.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			Command<COMMAND_DISARM_CAR_BOMB>(car);
			return 0;
		}
		else { throw "bad argument in function disarm_car_bomb"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int give_car_alarm(lua_State* L) {// (car) сигнализация.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			Command<COMMAND_GIVE_CAR_ALARM>(car);
			return 0;
		}
		else { throw "bad argument in function give_car_alarm"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

// ==================== ДОПОЛНИТЕЛЬНЫЕ ====================

int switch_car_siren(lua_State* L) {// (car, bool) сирена.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			bool flag = lua_toboolean(L, 2);
			Command<COMMAND_SWITCH_CAR_SIREN>(car, flag);
			return 0;
		}
		else { throw "bad argument in function switch_car_siren"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int set_driving_style_op(lua_State* L) {// (car, int) стиль вождения.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			int style = lua_tointeger(L, 2);
			Command<COMMAND_SET_CAR_DRIVING_STYLE>(car, style);
			return 0;
		}
		else { throw "bad argument in function set_driving_style_op"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int set_char_obj_wait(lua_State* L) { // нет задач (ждёт на ногах)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);
			ped->SetObjective(OBJECTIVE_WAIT_ON_FOOT);
			return 0;
		}
		else { throw "bad argument in function set_char_obj_wait"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_char_obj_flee_coord(lua_State* L) { // убежать от точки пешком до безопасности
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) &&
			LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) &&
			LUA_TNUMBER == lua_type(L, 4)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);
			float x = lua_tonumber(L, 2);
			float y = lua_tonumber(L, 3);
			float z = lua_tonumber(L, 4);
			CVector pos = { x, y, z };
			ped->SetObjective(OBJECTIVE_FLEE_ON_FOOT_TILL_SAFE, pos);
			return 0;
		}
		else { throw "bad argument in function set_char_obj_flee_coord"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_char_obj_guard_spot(lua_State* L) { // охранять точку
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) &&
			LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) &&
			LUA_TNUMBER == lua_type(L, 4)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);
			float x = lua_tonumber(L, 2);
			float y = lua_tonumber(L, 3);
			float z = lua_tonumber(L, 4);
			CVector pos = { x, y, z };
			ped->SetObjective(OBJECTIVE_GUARD_SPOT, pos);
			return 0;
		}
		else { throw "bad argument in function set_char_obj_guard_spot"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_char_obj_guard_area(lua_State* L) {// охранять зону (радиус).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) &&
			LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4) &&
			LUA_TNUMBER == lua_type(L, 5)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);
			float radius = lua_tonumber(L, 2);
			float x = lua_tonumber(L, 3);
			float y = lua_tonumber(L, 4);
			float z = lua_tonumber(L, 5);
			CVector pos = { x, y, z };
			ped->SetObjective(OBJECTIVE_GUARD_AREA, radius, pos);
			return 0;
		}
		else { throw "bad argument in function set_char_obj_guard_area"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int set_char_obj_wait_in_car(lua_State* L) { // ждать в машине
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);
			const void* p1 = lua_topointer(L, 2);
			CEntity* target = (CEntity*)p1;
			ped->SetObjective(OBJECTIVE_WAIT_IN_CAR, target);
			return 0;
		}
		else { throw "bad argument in function set_char_obj_wait_in_car"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_char_obj_leave_car(lua_State* L) { // выйти из машины
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);
			const void* p1 = lua_topointer(L, 2);
			CEntity* target = (CEntity*)p1;
			ped->SetObjective(OBJECTIVE_LEAVE_CAR, target);
			return 0;
		}
		else { throw "bad argument in function set_char_obj_leave_car"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_char_obj_enter_car_passenger(lua_State* L) { // сесть в авто как пассажир
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);
			const void* p1 = lua_topointer(L, 2);
			CEntity* target = (CEntity*)p1;
			ped->SetObjective(OBJECTIVE_ENTER_CAR_AS_PASSENGER, target);
			return 0;
		}
		else { throw "bad argument in function set_char_obj_enter_car_passenger"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_char_obj_enter_car_driver(lua_State* L) { // сесть в авто как водитель
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);
			const void* p1 = lua_topointer(L, 2);
			CEntity* target = (CEntity*)p1;
			ped->SetObjective(OBJECTIVE_ENTER_CAR_AS_DRIVER, target);
			return 0;
		}
		else { throw "bad argument in function set_char_obj_enter_car_driver"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_char_obj_follow_car_in_car(lua_State* L) { // следовать за авто из авто
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);
			const void* p1 = lua_topointer(L, 2);
			CEntity* target = (CEntity*)p1;
			ped->SetObjective(OBJECTIVE_FOLLOW_CAR_IN_CAR, target);
			return 0;
		}
		else { throw "bad argument in function set_char_obj_follow_car_in_car"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_char_obj_destroy_object(lua_State* L) { // уничтожить объект
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);
			const void* p1 = lua_topointer(L, 2);
			CEntity* target = (CEntity*)p1;
			ped->SetObjective(OBJECTIVE_DESTROY_OBJ, target);
			return 0;
		}
		else { throw "bad argument in function set_char_obj_destroy_object"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_char_obj_goto_area_in_car(lua_State* L) { // ехать к зоне
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) &&
			LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) &&
			LUA_TNUMBER == lua_type(L, 4)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);
			float x = lua_tonumber(L, 2);
			float y = lua_tonumber(L, 3);
			float z = lua_tonumber(L, 4);
			CVector pos = { x, y, z };
			ped->SetObjective(OBJECTIVE_GOTO_AREA_IN_CAR, pos);
			return 0;
		}
		else { throw "bad argument in function set_char_obj_goto_area_in_car"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_char_obj_follow_car_foot_offset(lua_State* L) {// следовать за авто пешком со смещением (01DC).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4) &&
			LUA_TNUMBER == lua_type(L, 5)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);
			const void* p1 = lua_topointer(L, 2);
			CVehicle* car = findcarinpool(p1);
			float x = lua_tonumber(L, 3);
			float y = lua_tonumber(L, 4);
			float z = lua_tonumber(L, 5);
			Command<COMMAND_SET_CHAR_OBJ_FOLLOW_CAR_ON_FOOT_WITH_OFFSET>(
				CPools::GetPedRef(ped), CPools::GetVehicleRef(car), x, y, z);
			return 0;
		}
		else { throw "bad argument in function set_char_obj_follow_car_foot_offset"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int set_char_obj_guard_attack(lua_State* L) { // охрана — атаковать противника
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);
			const void* p1 = lua_topointer(L, 2);
			CEntity* target = (CEntity*)p1;
			ped->SetObjective(OBJECTIVE_GUARD_ATTACK, target);
			return 0;
		}
		else { throw "bad argument in function set_char_obj_guard_attack"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_char_obj_goto_coord_on_foot(lua_State* L) { // идти к координатам пешком
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) &&
			LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) &&
			LUA_TNUMBER == lua_type(L, 4)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);
			float x = lua_tonumber(L, 2);
			float y = lua_tonumber(L, 3);
			float z = lua_tonumber(L, 4);
			CVector pos = { x, y, z };
			ped->SetObjective(OBJECTIVE_GOTO_AREA_ON_FOOT, pos);
			return 0;
		}
		else { throw "bad argument in function set_char_obj_goto_coord_on_foot"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_char_obj_goto_coord_in_car(lua_State* L) { // ехать к координатам
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) &&
			LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) &&
			LUA_TNUMBER == lua_type(L, 4)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);
			float x = lua_tonumber(L, 2);
			float y = lua_tonumber(L, 3);
			float z = lua_tonumber(L, 4);
			CVector pos = { x, y, z };
			ped->SetObjective(OBJECTIVE_GOTO_AREA_IN_CAR, pos);
			return 0;
		}
		else { throw "bad argument in function set_char_obj_goto_coord_in_car"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_char_obj_run_to_coord(lua_State* L) { // бежать к координатам
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) &&
			LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) &&
			LUA_TNUMBER == lua_type(L, 4)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);
			float x = lua_tonumber(L, 2);
			float y = lua_tonumber(L, 3);
			float z = lua_tonumber(L, 4);
			CVector pos = { x, y, z };
			ped->SetObjective(OBJECTIVE_RUN_TO_AREA, pos);
			return 0;
		}
		else { throw "bad argument in function set_char_obj_run_to_coord"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_char_obj_goto_area_any_means(lua_State* L) { // добираться любыми средствами
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) &&
			LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) &&
			LUA_TNUMBER == lua_type(L, 4)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);
			float x = lua_tonumber(L, 2);
			float y = lua_tonumber(L, 3);
			float z = lua_tonumber(L, 4);
			CVector pos = { x, y, z };
			ped->SetObjective(OBJECTIVE_GOTO_AREA_ANY_MEANS, pos);
			return 0;
		}
		else { throw "bad argument in function set_char_obj_goto_area_any_means"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_char_obj_follow_formation(lua_State* L) { // следовать в строю за лидером
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);
			const void* p1 = lua_topointer(L, 2);
			CEntity* target = (CEntity*)p1;
			ped->SetObjective(OBJECTIVE_FOLLOW_CHAR_IN_FORMATION, target);
			return 0;
		}
		else { throw "bad argument in function set_char_obj_follow_formation"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_char_obj_hail_taxi(lua_State* L) {// hail_taxi (0365).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);
			Command<COMMAND_SET_CHAR_OBJ_HAIL_TAXI>(CPools::GetPedRef(ped));
			return 0;
		}
		else { throw "bad argument in function set_char_obj_hail_taxi"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int set_char_obj_steal_any_car(lua_State* L) { // украсть любую машину
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);
			ped->SetObjective(OBJECTIVE_STEAL_ANY_CAR);
			return 0;
		}
		else { throw "bad argument in function set_char_obj_steal_any_car"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_char_obj_leave_any_car(lua_State* L) {// leave_any_car (03E2).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);
			Command<COMMAND_SET_CHAR_OBJ_LEAVE_ANY_CAR>(CPools::GetPedRef(ped));
			return 0;
		}
		else { throw "bad argument in function set_char_obj_leave_any_car"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int set_char_obj_flee_car(lua_State* L) { // убежать от машины
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);
			const void* p1 = lua_topointer(L, 2);
			CEntity* target = (CEntity*)p1;
			ped->SetObjective(OBJECTIVE_FLEE_CAR, target);
			return 0;
		}
		else { throw "bad argument in function set_char_obj_flee_car"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_char_obj_walk_to_char(lua_State* L) {// walk_to_char (04C2).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);
			const void* p1 = lua_topointer(L, 2);
			CPed* target = findpedinpool(p1);
			Command<COMMAND_SET_CHAR_OBJ_WALK_TO_CHAR>(CPools::GetPedRef(ped), CPools::GetPedRef(target));
			return 0;
		}
		else { throw "bad argument in function set_char_obj_walk_to_char"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int set_char_obj_steal_any_car_mission(lua_State* L) { // украсть любую миссионную машину
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);
			ped->SetObjective(OBJECTIVE_STEAL_ANY_MISSION_CAR);
			return 0;
		}
		else { throw "bad argument in function set_char_obj_steal_any_car_mission"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_char_obj_sprint_to_coord(lua_State* L) { // спринт к координатам
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) &&
			LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) &&
			LUA_TNUMBER == lua_type(L, 4)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);
			float x = lua_tonumber(L, 2);
			float y = lua_tonumber(L, 3);
			float z = lua_tonumber(L, 4);
			CVector pos = { x, y, z };
			ped->SetObjective(OBJECTIVE_SPRINT_TO_AREA, pos);
			return 0;
		}
		else { throw "bad argument in function set_char_obj_sprint_to_coord"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_char_obj_goto_car_on_foot(lua_State* L) {// goto_car_on_foot (0558).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);
			const void* p1 = lua_topointer(L, 2);
			CVehicle* car = findcarinpool(p1);
			Command<COMMAND_SET_CHAR_OBJ_GOTO_CAR_ON_FOOT>(CPools::GetPedRef(ped), CPools::GetVehicleRef(car));
			return 0;
		}
		else { throw "bad argument in function set_char_obj_goto_car_on_foot"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int get_projectile_count(lua_State* L) {// число активных снарядов CProjectileInfo.
	try {
		int count = 0;
		for (int i = 0; i < 32; i++) {
			if (CProjectileInfo::ms_apProjectile[i].bInUse)
				count++;
		}
		lua_pushinteger(L, count);
		return 1;
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int get_projectile_weapon(lua_State* L) {// тип оружия снаряда [0..31].
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {
			int idx = lua_tointeger(L, 1);
			if (idx >= 0 && idx < 32) {
				lua_pushinteger(L, CProjectileInfo::ms_apProjectile[idx].nWeaponId);
				return 1;
			}
			else throw "bad projectile index";
		}
		else throw "bad argument in function get_projectile_weapon";
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int get_projectile_source(lua_State* L) {// источник снаряда (CEntity*).
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {
			int idx = lua_tointeger(L, 1);
			if (idx >= 0 && idx < 32 && CProjectileInfo::ms_apProjectile[idx].bInUse) {
				lua_pushlightuserdata(L, CProjectileInfo::ms_apProjectile[idx].pSource);
				return 1;
			}
			else { lua_pushnil(L); return 1; }
		}
		else throw "bad argument in function get_projectile_source";
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int get_projectile_pos(lua_State* L) {// позиция снаряда [0..31] → x, y, z.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {
			int idx = lua_tointeger(L, 1);
			if (idx >= 0 && idx < 32 && CProjectileInfo::ms_apProjectile[idx].bInUse) {
				CVector& pos = CProjectileInfo::ms_apProjectile[idx].vecPos;
				lua_pushnumber(L, pos.x);
				lua_pushnumber(L, pos.y);
				lua_pushnumber(L, pos.z);
				return 3;
			}
		}
		else throw "bad argument in function get_projectile_pos";
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int set_projectile_pos(lua_State* L) {// уст позицию снаряда — управление ракетой в полёте.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {
			int idx = lua_tointeger(L, 1);
			if (idx >= 0 && idx < 32 && CProjectileInfo::ms_apProjectile[idx].bInUse) {
				float x = lua_tonumber(L, 2);
				float y = lua_tonumber(L, 3);
				float z = lua_tonumber(L, 4);
				CProjectileInfo::ms_apProjectile[idx].vecPos = { x, y, z };
			}
			return 0;
		}
		else throw "bad argument in function set_projectile_pos";
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int is_projectile_in_use(lua_State* L) {// слот снаряда [0..31] занят.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {
			int idx = lua_tointeger(L, 1);
			if (idx >= 0 && idx < 32) {
				lua_pushboolean(L, CProjectileInfo::ms_apProjectile[idx].bInUse);
				return 1;
			}
			else { lua_pushboolean(L, false); return 1; }
		}
		else throw "bad argument in function is_projectile_in_use";
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int remove_all_projectiles(lua_State* L) {// удалить все снаряды на карте.
	try {
		CProjectileInfo::RemoveAllProjectiles();
		return 0;
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int is_projectile_in_area(lua_State* L) {// снаряд в зоне (0x2EE) — 6 координат.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4) &&
			LUA_TNUMBER == lua_type(L, 5) && LUA_TNUMBER == lua_type(L, 6)) {
			float x1 = lua_tonumber(L, 1), y1 = lua_tonumber(L, 2), z1 = lua_tonumber(L, 3);
			float x2 = lua_tonumber(L, 4), y2 = lua_tonumber(L, 5), z2 = lua_tonumber(L, 6);
			bool result = Command<COMMAND_IS_PROJECTILE_IN_AREA>(x1, y1, z1, x2, y2, z2);
			lua_pushboolean(L, result);
			return 1;
		}
		else throw "bad argument in function is_projectile_in_area";
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int destroy_projectiles_in_area(lua_State* L) {// уничтожить все снаряды в зоне (0x2EF).
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4) &&
			LUA_TNUMBER == lua_type(L, 5) && LUA_TNUMBER == lua_type(L, 6)) {
			float x1 = lua_tonumber(L, 1), y1 = lua_tonumber(L, 2), z1 = lua_tonumber(L, 3);
			float x2 = lua_tonumber(L, 4), y2 = lua_tonumber(L, 5), z2 = lua_tonumber(L, 6);
			Command<COMMAND_DESTROY_PROJECTILES_IN_AREA>(x1, y1, z1, x2, y2, z2);
			return 0;
		}
		else throw "bad argument in function destroy_projectiles_in_area";
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int add_projectile(lua_State* L) {// создать снаряд (CEntity*, weapon, x,y,z, power).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4) &&
			LUA_TNUMBER == lua_type(L, 5) && LUA_TNUMBER == lua_type(L, 6)) {
			const void* p = lua_topointer(L, 1);
			CEntity* source = (CEntity*)p;
			int weapon = lua_tointeger(L, 2);
			float x = lua_tonumber(L, 3), y = lua_tonumber(L, 4), z = lua_tonumber(L, 5);
			float power = lua_tonumber(L, 6);
			CVector pos = { x, y, z };
			bool ok = CProjectileInfo::AddProjectile(source, (eWeaponType)weapon, pos, power);
			lua_pushboolean(L, ok);
			return 1;
		}
		else throw "bad argument in function add_projectile";
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int find_car_by_model(lua_State* L) {// найти первое авто по модели → lightuserdata.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {
			int model = lua_tointeger(L, 1);
			for (int i = 0; i < CPools::ms_pVehiclePool->m_nSize; i++) {
				CVehicle* car = CPools::ms_pVehiclePool->GetAt(i);
				if (car != nullptr && car->m_nModelIndex == model) {
					lua_pushlightuserdata(L, car);
					return 1;
				}
			}
			lua_pushnil(L);
			return 1;
		}
		else throw "bad argument in function find_car_by_model";
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int find_ped_by_model(lua_State* L) {// найти первого педа по модели → lightuserdata.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {
			int model = lua_tointeger(L, 1);
			for (int i = 0; i < CPools::ms_pPedPool->m_nSize; i++) {
				CPed* ped = CPools::ms_pPedPool->GetAt(i);
				if (ped != nullptr && ped->m_nModelIndex == model) {
					lua_pushlightuserdata(L, ped);
					return 1;
				}
			}
			lua_pushnil(L);
			return 1;
		}
		else throw "bad argument in function find_ped_by_model";
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int find_object_by_model(lua_State* L) {// найти объект по модели → lightuserdata.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {
			int model = lua_tointeger(L, 1);
			for (int i = 0; i < CPools::ms_pObjectPool->m_nSize; i++) {
				CObject* obj = CPools::ms_pObjectPool->GetAt(i);
				if (obj != nullptr && obj->m_nModelIndex == model) {
					lua_pushlightuserdata(L, obj);
					return 1;
				}
			}
			lua_pushnil(L);
			return 1;
		}
		else throw "bad argument in function find_object_by_model";
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int get_vehicle_pool_size(lua_State* L) {// размер пула транспорта.
	try {
		lua_pushinteger(L, CPools::ms_pVehiclePool->m_nSize);
		return 1;
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int get_car_pool_index(lua_State* L) {// индекс авто в пуле → int.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			int idx = CPools::ms_pVehiclePool->GetIndex(car);
			lua_pushinteger(L, idx);
			return 1;
		}
		else throw "bad argument in function get_car_pool_index";
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int get_car_by_pool_index(lua_State* L) {// авто по индексу в пуле → lightuserdata.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {
			int idx = lua_tointeger(L, 1);
			if (idx >= 0 && idx < CPools::ms_pVehiclePool->m_nSize &&
				!CPools::ms_pVehiclePool->IsFreeSlotAtIndex(idx)) {
				lua_pushlightuserdata(L, CPools::ms_pVehiclePool->GetAt(idx));
				return 1;
			}
			lua_pushnil(L);
			return 1;
		}
		else throw "bad argument in function get_car_by_pool_index";
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int get_car_velocity(lua_State* L) {// скорость авто m_vecMoveSpeed → x, y, z.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			lua_pushnumber(L, car->m_vecMoveSpeed.x);
			lua_pushnumber(L, car->m_vecMoveSpeed.y);
			lua_pushnumber(L, car->m_vecMoveSpeed.z);
			return 3;
		}
		else throw "bad argument in function get_car_velocity";
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int set_car_velocity(lua_State* L) {// уст скорость авто m_vecMoveSpeed.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) &&
			LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) &&
			LUA_TNUMBER == lua_type(L, 4)) {
			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);
			float x = lua_tonumber(L, 2);
			float y = lua_tonumber(L, 3);
			float z = lua_tonumber(L, 4);
			car->m_vecMoveSpeed = { x, y, z };
			return 0;
		}
		else throw "bad argument in function set_car_velocity";
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int remove_weapon_from_char(lua_State* L) {// удалить оружие у педа (0x555 REMOVE_WEAPON_FROM_CHAR).
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);
			int weapon = lua_tointeger(L, 2);
			Command<COMMAND_REMOVE_WEAPON_FROM_CHAR>(CPools::GetPedRef(ped), weapon);
			return 0;
		}
		else throw "bad argument in function remove_weapon_from_char";
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int get_forced_weather_type(lua_State* L) {// принудительный тип погоды (CWeather::ForcedWeatherType).
	try {
		lua_pushinteger(L, CWeather::ForcedWeatherType);
		return 1;
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int is_lightning_burst(lua_State* L) {// вспышка молнии (CWeather::LightningBurst).
	try {
		lua_pushboolean(L, CWeather::LightningBurst);
		return 1;
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int set_char_obj_kill_char_on_foot(lua_State* L) { // убить педа пешком (ближний бой)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			CPed* target = findpedinpool(lua_topointer(L, 2));
			Command<COMMAND_SET_CHAR_OBJ_KILL_CHAR_ON_FOOT>(CPools::GetPedRef(ped), CPools::GetPedRef(target));
			return 0;
		}
		else { throw "bad argument in function set_char_obj_kill_char_on_foot"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_char_obj_kill_char_any_means(lua_State* L) { // убить педа любыми средствами
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			CPed* target = findpedinpool(lua_topointer(L, 2));
			Command<COMMAND_SET_CHAR_OBJ_KILL_CHAR_ANY_MEANS>(CPools::GetPedRef(ped), CPools::GetPedRef(target));
			return 0;
		}
		else { throw "bad argument in function set_char_obj_kill_char_any_means"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_char_obj_flee_char_on_foot_till_safe(lua_State* L) { // убежать от педа до безопасного места
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			CPed* target = findpedinpool(lua_topointer(L, 2));
			Command<COMMAND_SET_CHAR_OBJ_FLEE_CHAR_ON_FOOT_TILL_SAFE>(CPools::GetPedRef(ped), CPools::GetPedRef(target));
			return 0;
		}
		else { throw "bad argument in function set_char_obj_flee_char_on_foot_till_safe"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_char_obj_flee_char_on_foot_always(lua_State* L) { // всегда убегать от педа
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			CPed* target = findpedinpool(lua_topointer(L, 2));
			Command<COMMAND_SET_CHAR_OBJ_FLEE_CHAR_ON_FOOT_ALWAYS>(CPools::GetPedRef(ped), CPools::GetPedRef(target));
			return 0;
		}
		else { throw "bad argument in function set_char_obj_flee_char_on_foot_always"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_char_obj_goto_char_on_foot(lua_State* L) { // идти к педу пешком
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			CPed* target = findpedinpool(lua_topointer(L, 2));
			Command<COMMAND_SET_CHAR_OBJ_GOTO_CHAR_ON_FOOT>(CPools::GetPedRef(ped), CPools::GetPedRef(target));
			return 0;
		}
		else { throw "bad argument in function set_char_obj_goto_char_on_foot"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_char_obj_aim_gun_at_char(lua_State* L) { // целиться из оружия в педа
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			CPed* target = findpedinpool(lua_topointer(L, 2));
			Command<COMMAND_SET_CHAR_OBJ_AIM_GUN_AT_CHAR>(CPools::GetPedRef(ped), CPools::GetPedRef(target));
			return 0;
		}
		else { throw "bad argument in function set_char_obj_aim_gun_at_char"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_char_obj_kill_player_on_foot(lua_State* L) { // убить игрока пешком
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			Command<COMMAND_SET_CHAR_OBJ_KILL_PLAYER_ON_FOOT>(CPools::GetPedRef(ped), CWorld::PlayerInFocus);
			return 0;
		}
		else { throw "bad argument in function set_char_obj_kill_player_on_foot"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_char_obj_kill_player_any_means(lua_State* L) { // убить игрока любыми средствами
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			Command<COMMAND_SET_CHAR_OBJ_KILL_PLAYER_ANY_MEANS>(CPools::GetPedRef(ped), CWorld::PlayerInFocus);
			return 0;
		}
		else { throw "bad argument in function set_char_obj_kill_player_any_means"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_char_obj_flee_player_on_foot_till_safe(lua_State* L) { // убежать от игрока до безопасного места
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			Command<COMMAND_SET_CHAR_OBJ_FLEE_PLAYER_ON_FOOT_TILL_SAFE>(CPools::GetPedRef(ped), CWorld::PlayerInFocus);
			return 0;
		}
		else { throw "bad argument in function set_char_obj_flee_player_on_foot_till_safe"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_char_obj_flee_player_on_foot_always(lua_State* L) { // всегда убегать от игрока
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			Command<COMMAND_SET_CHAR_OBJ_FLEE_PLAYER_ON_FOOT_ALWAYS>(CPools::GetPedRef(ped), CWorld::PlayerInFocus);
			return 0;
		}
		else { throw "bad argument in function set_char_obj_flee_player_on_foot_always"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_char_obj_goto_player_on_foot(lua_State* L) { // идти к игроку пешком
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			Command<COMMAND_SET_CHAR_OBJ_GOTO_PLAYER_ON_FOOT>(CPools::GetPedRef(ped), CWorld::PlayerInFocus);
			return 0;
		}
		else { throw "bad argument in function set_char_obj_goto_player_on_foot"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_char_obj_no_obj(lua_State* L) { // снять задачу с педа
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->SetObjective(OBJECTIVE_NO_OBJ);
			return 0;
		}
		else { throw "bad argument in function set_char_obj_no_obj"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_char_obj_catch_train(lua_State* L) { // сесть на поезд
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->SetObjective(OBJECTIVE_CATCH_TRAIN);
			return 0;
		}
		else { throw "bad argument in function set_char_obj_catch_train"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_char_obj_buy_ice_cream(lua_State* L) { // купить мороженое
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->SetObjective(OBJECTIVE_BUY_ICECREAM);
			return 0;
		}
		else { throw "bad argument in function set_char_obj_buy_ice_cream"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_char_obj_goto_area_on_foot(lua_State* L) { // идти к зоне пешком
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			CVector pos = { (float)lua_tonumber(L, 2), (float)lua_tonumber(L, 3), (float)lua_tonumber(L, 4) };
			ped->SetObjective(OBJECTIVE_GOTO_AREA_ON_FOOT, pos);
			return 0;
		}
		else { throw "bad argument in function set_char_obj_goto_area_on_foot"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_char_obj_destroy_car(lua_State* L) { // уничтожить машину
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			CEntity* target = (CEntity*)findcarinpool(lua_topointer(L, 2));
			ped->SetObjective(OBJECTIVE_DESTROY_CAR, target);
			return 0;
		}
		else { throw "bad argument in function set_char_obj_destroy_car"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_char_obj_fire_at_object_from_vehicle(lua_State* L) { // стрелять по объекту из авто
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			CEntity* target = (CEntity*)findobjinpool(lua_topointer(L, 2));
			ped->SetObjective(OBJECTIVE_FIRE_AT_OBJ_FROM_VEHICLE, target);
			return 0;
		}
		else { throw "bad argument in function set_char_obj_fire_at_object_from_vehicle"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_char_obj_follow_route(lua_State* L) { // следовать по маршруту
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			int route = (int)lua_tointeger(L, 2);
			int speed = (int)lua_tointeger(L, 3);
			Command<COMMAND_SET_CHAR_OBJ_FOLLOW_ROUTE>(CPools::GetPedRef(ped), route, speed);
			return 0;
		}
		else { throw "bad argument in function set_char_obj_follow_route"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_heli_forced_z(lua_State* L) { // принудительная высота вертолёта (зависание)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_HELI) throw "not a heli";
			CHeli* heli = (CHeli*)car;
			lua_pushnumber(L, heli->m_fHeliForcedZ);
			return 1;
		}
		else { throw "bad argument in function get_heli_forced_z"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_heli_forced_z(lua_State* L) { // уст. принудительная высота вертолёта (зависание)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_HELI) throw "not a heli";
			CHeli* heli = (CHeli*)car;
			heli->m_fHeliForcedZ = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_heli_forced_z"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_heli_searchlight_x(lua_State* L) { // координата X прожектора вертолёта
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_HELI) throw "not a heli";
			CHeli* heli = (CHeli*)car;
			lua_pushnumber(L, heli->m_fSearchLightX);
			return 1;
		}
		else { throw "bad argument in function get_heli_searchlight_x"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_heli_searchlight_x(lua_State* L) { // уст. координата X прожектора вертолёта
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_HELI) throw "not a heli";
			CHeli* heli = (CHeli*)car;
			heli->m_fSearchLightX = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_heli_searchlight_x"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_heli_searchlight_y(lua_State* L) { // координата Y прожектора вертолёта
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_HELI) throw "not a heli";
			CHeli* heli = (CHeli*)car;
			lua_pushnumber(L, heli->m_fSearchLightY);
			return 1;
		}
		else { throw "bad argument in function get_heli_searchlight_y"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_heli_searchlight_y(lua_State* L) { // уст. координата Y прожектора вертолёта
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_HELI) throw "not a heli";
			CHeli* heli = (CHeli*)car;
			heli->m_fSearchLightY = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_heli_searchlight_y"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_heli_searchlight_intensity(lua_State* L) { // яркость прожектора вертолёта (0..1)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_HELI) throw "not a heli";
			CHeli* heli = (CHeli*)car;
			lua_pushnumber(L, heli->m_fSearchLightIntensity);
			return 1;
		}
		else { throw "bad argument in function get_heli_searchlight_intensity"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_heli_searchlight_intensity(lua_State* L) { // уст. яркость прожектора вертолёта (0..1)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_HELI) throw "not a heli";
			CHeli* heli = (CHeli*)car;
			heli->m_fSearchLightIntensity = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_heli_searchlight_intensity"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_heli_rotor_angle(lua_State* L) { // угол вращения винта вертолёта
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_HELI) throw "not a heli";
			CHeli* heli = (CHeli*)car;
			lua_pushnumber(L, heli->m_fRotorAngle);
			return 1;
		}
		else { throw "bad argument in function get_heli_rotor_angle"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_heli_rotor_angle(lua_State* L) { // уст. угол вращения винта вертолёта
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_HELI) throw "not a heli";
			CHeli* heli = (CHeli*)car;
			heli->m_fRotorAngle = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_heli_rotor_angle"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int heli_send_down_swat(lua_State* L) { // спецназ спускается с вертолёта на верёвке
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_HELI) throw "not a heli";
			CHeli* heli = (CHeli*)car;
			heli->SendDownSwat();
			return 0;
		}
		else { throw "bad argument in function heli_send_down_swat"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int heli_get_num_random(lua_State* L) { // число случайных вертолётов
	lua_pushinteger(L, (lua_Integer)CHeli::NumRandomHelis);
	return 1;
};
int heli_set_num_random(lua_State* L) { // уст. число случайных вертолётов
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {
			CHeli::NumRandomHelis = (unsigned int)lua_tointeger(L, 1);
			return 0;
		}
		else { throw "bad argument in function heli_set_num_random"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int heli_is_catalina_on(lua_State* L) { // вертолёт Катарины активен
	lua_pushboolean(L, CHeli::CatalinaHeliOn);
	return 1;
};
int heli_set_catalina_on(lua_State* L) { // вкл/выкл вертолёт Катарины
	try {
		if (LUA_TBOOLEAN == lua_type(L, 1)) {
			CHeli::CatalinaHeliOn = (bool)lua_toboolean(L, 1);
			return 0;
		}
		else { throw "bad argument in function heli_set_catalina_on"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_velocity(lua_State* L) { // скорость педа m_vecMoveSpeed (x,y,z)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPhysical* ent = (CPhysical*)findpedinpool(lua_topointer(L, 1));
			lua_pushnumber(L, ent->m_vecMoveSpeed.x);
			lua_pushnumber(L, ent->m_vecMoveSpeed.y);
			lua_pushnumber(L, ent->m_vecMoveSpeed.z);
			return 3;
		}
		else { throw "bad argument in function get_ped_velocity"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_velocity(lua_State* L) { // уст. скорость педа m_vecMoveSpeed (x,y,z)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {
			CPhysical* ent = (CPhysical*)findpedinpool(lua_topointer(L, 1));
			ent->m_vecMoveSpeed.x = (float)lua_tonumber(L, 2);
			ent->m_vecMoveSpeed.y = (float)lua_tonumber(L, 3);
			ent->m_vecMoveSpeed.z = (float)lua_tonumber(L, 4);
			return 0;
		}
		else { throw "bad argument in function set_ped_velocity"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_object_velocity(lua_State* L) { // скорость объекта m_vecMoveSpeed (x,y,z)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPhysical* ent = (CPhysical*)findobjinpool(lua_topointer(L, 1));
			lua_pushnumber(L, ent->m_vecMoveSpeed.x);
			lua_pushnumber(L, ent->m_vecMoveSpeed.y);
			lua_pushnumber(L, ent->m_vecMoveSpeed.z);
			return 3;
		}
		else { throw "bad argument in function get_object_velocity"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_object_velocity(lua_State* L) { // уст. скорость объекта m_vecMoveSpeed (x,y,z)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {
			CPhysical* ent = (CPhysical*)findobjinpool(lua_topointer(L, 1));
			ent->m_vecMoveSpeed.x = (float)lua_tonumber(L, 2);
			ent->m_vecMoveSpeed.y = (float)lua_tonumber(L, 3);
			ent->m_vecMoveSpeed.z = (float)lua_tonumber(L, 4);
			return 0;
		}
		else { throw "bad argument in function set_object_velocity"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_speed(lua_State* L) { // скалярная скорость авто (м/с)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPhysical* ent = (CPhysical*)findcarinpool(lua_topointer(L, 1));
			float sp = ent->m_vecMoveSpeed.Magnitude();
			lua_pushnumber(L, sp);
			return 1;
		}
		else { throw "bad argument in function get_car_speed"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_speed(lua_State* L) { // скалярная скорость педа (м/с)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPhysical* ent = (CPhysical*)findpedinpool(lua_topointer(L, 1));
			float sp = ent->m_vecMoveSpeed.Magnitude();
			lua_pushnumber(L, sp);
			return 1;
		}
		else { throw "bad argument in function get_ped_speed"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_object_speed(lua_State* L) { // скалярная скорость объекта (м/с)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPhysical* ent = (CPhysical*)findobjinpool(lua_topointer(L, 1));
			float sp = ent->m_vecMoveSpeed.Magnitude();
			lua_pushnumber(L, sp);
			return 1;
		}
		else { throw "bad argument in function get_object_speed"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int add_ammo_to_char(lua_State* L) { // добавить патронов педу в оружие
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			int weapon = (int)lua_tointeger(L, 2);
			int ammo = (int)lua_tointeger(L, 3);
			Command<COMMAND_ADD_AMMO_TO_CHAR>(CPools::GetPedRef(ped), weapon, ammo);
			return 0;
		}
		else { throw "bad argument in function add_ammo_to_char"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_char_ammo(lua_State* L) { // установить патроны педу в оружие
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			int weapon = (int)lua_tointeger(L, 2);
			int ammo = (int)lua_tointeger(L, 3);
			Command<COMMAND_SET_CHAR_AMMO>(CPools::GetPedRef(ped), weapon, ammo);
			return 0;
		}
		else { throw "bad argument in function set_char_ammo"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ammo_in_char_weapon(lua_State* L) { // патроны в оружии педа
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			int weapon = (int)lua_tointeger(L, 2);
			int ammo = 0;
			Command<COMMAND_GET_AMMO_IN_CHAR_WEAPON>(CPools::GetPedRef(ped), weapon, &ammo);
			lua_pushinteger(L, (lua_Integer)ammo);
			return 1;
		}
		else { throw "bad argument in function get_ammo_in_char_weapon"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_char_weapon_in_slot(lua_State* L) { // оружие в слоте педа
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			int slot = (int)lua_tointeger(L, 2);
			int weapon = 0;
			Command<COMMAND_GET_CHAR_WEAPON_IN_SLOT>(CPools::GetPedRef(ped), slot, &weapon);
			lua_pushinteger(L, (lua_Integer)weapon);
			return 1;
		}
		else { throw "bad argument in function get_char_weapon_in_slot"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_player_fast_reload(lua_State* L) { // быстрая перезарядка игроку
	try {
		if (LUA_TBOOLEAN == lua_type(L, 1)) {
			Command<COMMAND_SET_PLAYER_FAST_RELOAD>(CWorld::PlayerInFocus, (bool)lua_toboolean(L, 1));
			return 0;
		}
		else { throw "bad argument in function set_player_fast_reload"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int is_char_shooting_in_area(lua_State* L) { // пед стреляет в зоне
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4) &&
			LUA_TNUMBER == lua_type(L, 5) && LUA_TBOOLEAN == lua_type(L, 6)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			float x1 = (float)lua_tonumber(L, 2), y1 = (float)lua_tonumber(L, 3);
			float x2 = (float)lua_tonumber(L, 4), y2 = (float)lua_tonumber(L, 5);
			bool sphere = (bool)lua_toboolean(L, 6);
			bool r = Command<COMMAND_IS_CHAR_SHOOTING_IN_AREA>(CPools::GetPedRef(ped), x1, y1, x2, y2, sphere);
			lua_pushboolean(L, r);
			return 1;
		}
		else { throw "bad argument in function is_char_shooting_in_area"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_char_stop_shoot_dont_seek_entity(lua_State* L) { // пед перестаёт стрелять и не ищет цель
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			Command<COMMAND_SET_CHAR_STOP_SHOOT_DONT_SEEK_ENTITY>(CPools::GetPedRef(ped), (bool)lua_toboolean(L, 1));
			return 0;
		}
		else { throw "bad argument in function set_char_stop_shoot_dont_seek_entity"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};

int create_rope_on_cords_with_swat(lua_State* L) { // создать веревку на координатах с бойцом спецназа.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {// значение число.
			float x = lua_tonumber(L, 1); float y = lua_tonumber(L, 2);
			float z = lua_tonumber(L, 3); CVector pos = { x, y, z };
			Command<COMMAND_CREATE_SWAT_ROPE>(pos.x, pos.y, pos.z);
			return 0;
		}
		else { throw "bad argument in function create_rope_on_cords_with_swat"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};
int add_bullet_trace_radius(lua_State* L) { // след пули: от точки к точке (радиус, время, прозрачность)
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) &&
			LUA_TNUMBER == lua_type(L, 4) && LUA_TNUMBER == lua_type(L, 5) && LUA_TNUMBER == lua_type(L, 6) &&
			LUA_TNUMBER == lua_type(L, 7) && LUA_TNUMBER == lua_type(L, 8) && LUA_TNUMBER == lua_type(L, 9)) {
			CVector origin = { (float)lua_tonumber(L,1), (float)lua_tonumber(L,2), (float)lua_tonumber(L,3) };
			CVector target = { (float)lua_tonumber(L,4), (float)lua_tonumber(L,5), (float)lua_tonumber(L,6) };
			float radius = (float)lua_tonumber(L,7);
			unsigned int time = (unsigned int)lua_tointeger(L,8);
			unsigned char transparency = (unsigned char)lua_tointeger(L,9);
			CBulletTraces::AddTrace(&origin, &target, radius, time, transparency);
		}
		else { throw "bad argument in function add_bullet_trace_radius"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int add_bullet_trace_weapon(lua_State* L) { // след пули: оружием и сущностью
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) &&
			LUA_TNUMBER == lua_type(L, 4) && LUA_TNUMBER == lua_type(L, 5) && LUA_TNUMBER == lua_type(L, 6) &&
			LUA_TNUMBER == lua_type(L, 7) && LUA_TLIGHTUSERDATA == lua_type(L, 8)) {
			CVector origin = { (float)lua_tonumber(L,1), (float)lua_tonumber(L,2), (float)lua_tonumber(L,3) };
			CVector target = { (float)lua_tonumber(L,4), (float)lua_tonumber(L,5), (float)lua_tonumber(L,6) };
			int weapon = (int)lua_tointeger(L,7);
			CEntity* entity = (CEntity*)lua_topointer(L,8);
			CBulletTraces::AddTrace(&origin, &target, weapon, entity);
		}
		else { throw "bad argument in function add_bullet_trace_weapon"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_bullet_trace_count(lua_State* L) { // число активных следов пуль
	int count = 0;
	for (int i = 0; i < 16; i++)
		if (CBulletTraces::aTraces[i].m_bExist) count++;
	lua_pushinteger(L, (lua_Integer)count);
	return 1;
};
int get_bullet_trace_origin(lua_State* L) { // точка начала следа пули
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {
			int i = (int)lua_tointeger(L, 1);
			if (i < 0 || i > 15) throw "index out of range";
			lua_pushnumber(L, CBulletTraces::aTraces[i].m_vecOrigin.x);
			lua_pushnumber(L, CBulletTraces::aTraces[i].m_vecOrigin.y);
			lua_pushnumber(L, CBulletTraces::aTraces[i].m_vecOrigin.z);
			return 3;
		}
		else { throw "bad argument in function get_bullet_trace_origin"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_bullet_trace_target(lua_State* L) { // точка конца следа пули
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {
			int i = (int)lua_tointeger(L, 1);
			if (i < 0 || i > 15) throw "index out of range";
			lua_pushnumber(L, CBulletTraces::aTraces[i].m_vecTarget.x);
			lua_pushnumber(L, CBulletTraces::aTraces[i].m_vecTarget.y);
			lua_pushnumber(L, CBulletTraces::aTraces[i].m_vecTarget.z);
			return 3;
		}
		else { throw "bad argument in function get_bullet_trace_target"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int clear_bullet_traces(lua_State* L) { // удалить все следы пуль
	CBulletTraces::Init();
	return 0;
};
int place_3d_marker(lua_State* L) { // 3D-маркер (стрелка/цилиндр)
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) &&
			LUA_TNUMBER == lua_type(L, 4) && LUA_TNUMBER == lua_type(L, 5) && LUA_TNUMBER == lua_type(L, 6) &&
			LUA_TNUMBER == lua_type(L, 7) && LUA_TNUMBER == lua_type(L, 8) && LUA_TNUMBER == lua_type(L, 9) &&
			LUA_TNUMBER == lua_type(L, 10) && LUA_TNUMBER == lua_type(L, 11) && LUA_TNUMBER == lua_type(L, 12) &&
			LUA_TNUMBER == lua_type(L, 13)) {
			unsigned int id = (unsigned int)lua_tointeger(L, 1);
			unsigned short type = (unsigned short)lua_tointeger(L, 2);
			CVector pos = { (float)lua_tonumber(L, 3), (float)lua_tonumber(L, 4), (float)lua_tonumber(L, 5) };
			float size = (float)lua_tonumber(L, 6);
			unsigned char r = (unsigned char)lua_tointeger(L, 7);
			unsigned char g = (unsigned char)lua_tointeger(L, 8);
			unsigned char b = (unsigned char)lua_tointeger(L, 9);
			unsigned char a = (unsigned char)lua_tointeger(L, 10);
			unsigned short pulsePeriod = (unsigned short)lua_tointeger(L, 11);
			float pulseFraction = (float)lua_tonumber(L, 12);
			short rotateRate = (short)lua_tointeger(L, 13);
			C3dMarkers::PlaceMarker(id, type, pos, size, r, g, b, a, pulsePeriod, pulseFraction, rotateRate);
		}
		else { throw "bad argument in function place_3d_marker"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int place_3d_marker_set(lua_State* L) { // набор 3D-маркеров
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) &&
			LUA_TNUMBER == lua_type(L, 4) && LUA_TNUMBER == lua_type(L, 5) && LUA_TNUMBER == lua_type(L, 6) &&
			LUA_TNUMBER == lua_type(L, 7) && LUA_TNUMBER == lua_type(L, 8) && LUA_TNUMBER == lua_type(L, 9) &&
			LUA_TNUMBER == lua_type(L, 10) && LUA_TNUMBER == lua_type(L, 11) && LUA_TNUMBER == lua_type(L, 12) &&
			LUA_TNUMBER == lua_type(L, 13)) {
			unsigned int id = (unsigned int)lua_tointeger(L, 1);
			unsigned short type = (unsigned short)lua_tointeger(L, 2);
			CVector pos = { (float)lua_tonumber(L, 3), (float)lua_tonumber(L, 4), (float)lua_tonumber(L, 5) };
			float size = (float)lua_tonumber(L, 6);
			unsigned char r = (unsigned char)lua_tointeger(L, 7);
			unsigned char g = (unsigned char)lua_tointeger(L, 8);
			unsigned char b = (unsigned char)lua_tointeger(L, 9);
			unsigned char a = (unsigned char)lua_tointeger(L, 10);
			unsigned short pulsePeriod = (unsigned short)lua_tointeger(L, 11);
			float pulseFraction = (float)lua_tonumber(L, 12);
			short rotateRate = (short)lua_tointeger(L, 13);
			C3dMarkers::PlaceMarkerSet(id, type, pos, size, r, g, b, a, pulsePeriod, pulseFraction, rotateRate);
		}
		else { throw "bad argument in function place_3d_marker_set"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_num_active_3d_markers(lua_State* L) { // число активных 3D-маркеров
	lua_pushinteger(L, (lua_Integer)C3dMarkers::NumActiveMarkers);
	return 1;
};
int get_pedstat_m_fFleeDistance(lua_State* L) { // дистанция бегства педа
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushnumber(L, ped->m_pPedStats->m_fFleeDistance);
			return 1;
		}
		else { throw "bad argument in function get_pedstat_m_fFleeDistance"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_pedstat_m_fFleeDistance(lua_State* L) { // уст. дистанция бегства педа
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_pPedStats->m_fFleeDistance = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_pedstat_m_fFleeDistance"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_pedstat_m_fHeadingChangeRate(lua_State* L) { // скорость поворота педа
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushnumber(L, ped->m_pPedStats->m_fHeadingChangeRate);
			return 1;
		}
		else { throw "bad argument in function get_pedstat_m_fHeadingChangeRate"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_pedstat_m_fHeadingChangeRate(lua_State* L) { // уст. скорость поворота педа
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_pPedStats->m_fHeadingChangeRate = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_pedstat_m_fHeadingChangeRate"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_pedstat_m_fAttackStrength(lua_State* L) { // сила атаки педа
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushnumber(L, ped->m_pPedStats->m_fAttackStrength);
			return 1;
		}
		else { throw "bad argument in function get_pedstat_m_fAttackStrength"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_pedstat_m_fAttackStrength(lua_State* L) { // уст. сила атаки педа
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_pPedStats->m_fAttackStrength = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_pedstat_m_fAttackStrength"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_pedstat_m_fDefendWeakness(lua_State* L) { // слабость защиты педа
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushnumber(L, ped->m_pPedStats->m_fDefendWeakness);
			return 1;
		}
		else { throw "bad argument in function get_pedstat_m_fDefendWeakness"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_pedstat_m_fDefendWeakness(lua_State* L) { // уст. слабость защиты педа
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_pPedStats->m_fDefendWeakness = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_pedstat_m_fDefendWeakness"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_door_status(lua_State* L) { // состояние двери авто
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_AUTOMOBILE) throw "not an automobile";
			CAutomobile* auto_ = (CAutomobile*)car;
			lua_pushinteger(L, (lua_Integer)auto_->m_carDamage.GetDoorStatus((eDoors)lua_tointeger(L, 2)));
			return 1;
		}
		else { throw "bad argument in function get_car_door_status"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_door_status(lua_State* L) { // уст. состояние двери авто
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_AUTOMOBILE) throw "not an automobile";
			CAutomobile* auto_ = (CAutomobile*)car;
			auto_->m_carDamage.SetDoorStatus((eDoors)lua_tointeger(L, 2), (unsigned int)lua_tointeger(L, 3));
		}
		else { throw "bad argument in function set_car_door_status"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_panel_status(lua_State* L) { // состояние панели авто
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_AUTOMOBILE) throw "not an automobile";
			CAutomobile* auto_ = (CAutomobile*)car;
			lua_pushinteger(L, (lua_Integer)auto_->m_carDamage.GetPanelStatus((ePanels)lua_tointeger(L, 2)));
			return 1;
		}
		else { throw "bad argument in function get_car_panel_status"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_light_status(lua_State* L) { // состояние фары авто
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_AUTOMOBILE) throw "not an automobile";
			CAutomobile* auto_ = (CAutomobile*)car;
			lua_pushinteger(L, (lua_Integer)auto_->m_carDamage.GetLightStatus((eLights)lua_tointeger(L, 2)));
			return 1;
		}
		else { throw "bad argument in function get_car_light_status"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_light_status(lua_State* L) { // уст. состояние фары авто
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_AUTOMOBILE) throw "not an automobile";
			CAutomobile* auto_ = (CAutomobile*)car;
			auto_->m_carDamage.SetLightStatus((eLights)lua_tointeger(L, 2), (unsigned int)lua_tointeger(L, 3));
		}
		else { throw "bad argument in function set_car_light_status"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_wheel_status(lua_State* L) { // состояние колеса авто
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_AUTOMOBILE) throw "not an automobile";
			CAutomobile* auto_ = (CAutomobile*)car;
			lua_pushinteger(L, (lua_Integer)auto_->m_carDamage.GetWheelStatus((int)lua_tointeger(L, 2)));
			return 1;
		}
		else { throw "bad argument in function get_car_wheel_status"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_wheel_status(lua_State* L) { // уст. состояние колеса авто
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_AUTOMOBILE) throw "not an automobile";
			CAutomobile* auto_ = (CAutomobile*)car;
			auto_->m_carDamage.SetWheelStatus((int)lua_tointeger(L, 2), (unsigned int)lua_tointeger(L, 3));
		}
		else { throw "bad argument in function set_car_wheel_status"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_engine_status(lua_State* L) { // состояние двигателя авто
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_AUTOMOBILE) throw "not an automobile";
			CAutomobile* auto_ = (CAutomobile*)car;
			lua_pushinteger(L, (lua_Integer)auto_->m_carDamage.GetEngineStatus());
			return 1;
		}
		else { throw "bad argument in function get_car_engine_status"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_engine_status(lua_State* L) { // уст. состояние двигателя авто
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_AUTOMOBILE) throw "not an automobile";
			CAutomobile* auto_ = (CAutomobile*)car;
			auto_->m_carDamage.SetEngineStatus((unsigned int)lua_tointeger(L, 2));
		}
		else { throw "bad argument in function set_car_engine_status"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int apply_car_damage(lua_State* L) { // повредить компонент авто
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_AUTOMOBILE) throw "not an automobile";
			CAutomobile* auto_ = (CAutomobile*)car;
			float intensity = (float)lua_tonumber(L, 3);
			auto_->m_carDamage.ApplyDamage((tComponent)lua_tointeger(L, 2), intensity, 0.0f);
		}
		else { throw "bad argument in function apply_car_damage"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int fuck_car_completely(lua_State* L) { // уничтожить авто визуально
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_AUTOMOBILE) throw "not an automobile";
			((CAutomobile*)car)->m_carDamage.FuckCarCompletely();
			return 0;
		}
		else { throw "bad argument in function fuck_car_completely"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int reset_car_damage(lua_State* L) { // сбросить повреждения авто
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_AUTOMOBILE) throw "not an automobile";
			((CAutomobile*)car)->m_carDamage.ResetDamageStatus();
			return 0;
		}
		else { throw "bad argument in function reset_car_damage"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int add_AutoPaintingBudget(lua_State* L) { // бюджет на покраску авто
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {
			CStats::AutoPaintingBudget += (float)lua_tonumber(L, 1);
			return 0;
		}
		else { throw "bad argument in function add_AutoPaintingBudget"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int add_PropertyBudget(lua_State* L) { // бюджет на недвижимость
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {
			CStats::PropertyBudget += (float)lua_tonumber(L, 1);
			return 0;
		}
		else { throw "bad argument in function add_PropertyBudget"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int add_FashionBudget(lua_State* L) { // бюджет на одежду
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {
			CStats::FashionBudget += (float)lua_tonumber(L, 1);
			return 0;
		}
		else { throw "bad argument in function add_FashionBudget"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int add_WeaponBudget(lua_State* L) { // бюджет на оружие
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {
			CStats::WeaponBudget += (float)lua_tonumber(L, 1);
			return 0;
		}
		else { throw "bad argument in function add_WeaponBudget"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int add_BloodRingKills(lua_State* L) { // убийства в Bloodring
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {
			CStats::BloodRingKills += (float)lua_tonumber(L, 1);
			return 0;
		}
		else { throw "bad argument in function add_BloodRingKills"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int add_PizzasDelivered(lua_State* L) { // доставленные пиццы
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {
			CStats::PizzasDelivered += (float)lua_tonumber(L, 1);
			return 0;
		}
		else { throw "bad argument in function add_PizzasDelivered"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int add_Assassinations(lua_State* L) { // заказные убийства
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {
			CStats::Assassinations += (float)lua_tonumber(L, 1);
			return 0;
		}
		else { throw "bad argument in function add_Assassinations"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int add_MovieStunts(lua_State* L) { // киноскунты
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {
			CStats::MovieStunts += (float)lua_tonumber(L, 1);
			return 0;
		}
		else { throw "bad argument in function add_MovieStunts"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int add_LoanSharks(lua_State* L) { // визиты к ростовщикам
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {
			CStats::LoanSharks += (float)lua_tonumber(L, 1);
			return 0;
		}
		else { throw "bad argument in function add_LoanSharks"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int clear_all_char_anims(lua_State* L) { // очистить все анимации педа
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			Command<COMMAND_CLEAR_ALL_CHAR_ANIMS>(CPools::GetPedRef(ped));
		}
		else { throw "bad argument in function clear_all_char_anims"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int clear_char_follow_path(lua_State* L) { // очистить маршрут педа
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			Command<COMMAND_CLEAR_CHAR_FOLLOW_PATH>(CPools::GetPedRef(ped));
		}
		else { throw "bad argument in function clear_char_follow_path"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int clear_char_last_damage_entity(lua_State* L) { // забыть последнюю повредившую сущность
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			Command<COMMAND_CLEAR_CHAR_LAST_DAMAGE_ENTITY>(CPools::GetPedRef(ped));
		}
		else { throw "bad argument in function clear_char_last_damage_entity"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int clear_char_last_weapon_damage(lua_State* L) { // забыть последнее оружие урона
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			Command<COMMAND_CLEAR_CHAR_LAST_WEAPON_DAMAGE>(CPools::GetPedRef(ped));
		}
		else { throw "bad argument in function clear_char_last_weapon_damage"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int char_set_idle(lua_State* L) { // пед в режим ожидания
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			Command<COMMAND_CHAR_SET_IDLE>(CPools::GetPedRef(ped));
		}
		else { throw "bad argument in function char_set_idle"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int add_armour_to_char(lua_State* L) { // добавить брони педу
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			Command<COMMAND_ADD_ARMOUR_TO_CHAR>(CPools::GetPedRef(ped), (float)lua_tonumber(L, 2));
		}
		else { throw "bad argument in function add_armour_to_char"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int add_armour_to_player(lua_State* L) { // добавить брони игроку
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {
			Command<COMMAND_ADD_ARMOUR_TO_PLAYER>(CWorld::PlayerInFocus, (float)lua_tonumber(L, 1));
		}
		else { throw "bad argument in function add_armour_to_player"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int add_ammo_to_player(lua_State* L) { // добавить патронов игроку
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			Command<COMMAND_ADD_AMMO_TO_PLAYER>(CWorld::PlayerInFocus, (int)lua_tointeger(L, 1), (int)lua_tointeger(L, 2));
		}
		else { throw "bad argument in function add_ammo_to_player"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int clear_area_of_cars(lua_State* L) { // очистить зону от машин
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {
			float x1 = (float)lua_tonumber(L, 1), y1 = (float)lua_tonumber(L, 2);
			float x2 = (float)lua_tonumber(L, 3), y2 = (float)lua_tonumber(L, 4);
			Command<COMMAND_CLEAR_AREA_OF_CARS>(x1, y1, x2, y2);
		}
		else { throw "bad argument in function clear_area_of_cars"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int clear_area_of_chars(lua_State* L) { // очистить зону от педов
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {
			float x1 = (float)lua_tonumber(L, 1), y1 = (float)lua_tonumber(L, 2);
			float x2 = (float)lua_tonumber(L, 3), y2 = (float)lua_tonumber(L, 4);
			Command<COMMAND_CLEAR_AREA_OF_CHARS>(x1, y1, x2, y2);
		}
		else { throw "bad argument in function clear_area_of_chars"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};

int create_rope_on_cords(lua_State* L) { // создать веревку на координатах.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {// значение число.

			float x = lua_tonumber(L, 1); float y = lua_tonumber(L, 2);
			float z = lua_tonumber(L, 3);
			CVector pos = { x, y, z };
			//((char(__cdecl*)(float,float,float))0x632550)(x, y, z);
			//адрес 00632550
			//char __cdecl CRopes::CreateRopeWithSwatComingDown(float a1, float a2, float a3)

			((int(__cdecl*)())0x633000)();

			//	адрес 0x633000
			//__int16 __cdecl CRopes::Update()

			/*
 адрес 00632550
 char __cdecl CRopes::CreateRopeWithSwatComingDown(float a1, float a2, float a3)
{
  int v4; // eax

  if ( !byte_786DB4 )
  {
	byte_786DB4 = 1;
	dword_786DB0 = 0;
  }
  if ( CStreaming::ms_aInfoForModel[2].m_nLoadState != 1 )
	return 0;
  if ( !CRopes::RegisterRope(dword_786DB0 + 100, a1, a2, a3, 1) )
	return 0;
  v4 = CPopulation::AddPed(6, 3, &a1, 0);
  *(_BYTE *)(v4 + 81) &= 0xFEu;
  *(_DWORD *)(v4 + 1552) = 1;
  *(_DWORD *)(v4 + 1556) = dword_786DB0 + 100;
  CAnimManager::BlendAnimation(*(_DWORD *)(v4 + 76), 0, 172, dword_6DC380);
  dword_786DB0 = (unsigned __int8)(dword_786DB0 + 1);
  return 1;
}
*/
	return 0;}// int

		else { throw "bad argument in function create_rope_on_cords"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};
int get_population_total_peds(lua_State* L) { // всего педов в мире
	lua_pushnumber(L, (double)(CPopulation::ms_nTotalPeds));
	return 1;};
int get_population_civ_peds(lua_State* L) { // мирных педов
	lua_pushnumber(L, (double)(CPopulation::ms_nTotalCivPeds));
	return 1;};
int get_population_gang_peds(lua_State* L) { // бандитских педов
	lua_pushnumber(L, (double)(CPopulation::ms_nTotalGangPeds));
	return 1;};
int get_population_car_passengers(lua_State* L) { // пассажиров в авто
	lua_pushnumber(L, (double)(CPopulation::ms_nTotalCarPassengerPeds));
	return 1;};
int get_population_mission_peds(lua_State* L) { // миссионных педов
	lua_pushnumber(L, (double)(CPopulation::ms_nTotalMissionPeds));
	return 1;};
int get_population_civ_male(lua_State* L) { // мужчин-мирных
	lua_pushnumber(L, (double)(CPopulation::ms_nNumCivMale));
	return 1;};
int get_population_civ_female(lua_State* L) { // женщин-мирных
	lua_pushnumber(L, (double)(CPopulation::ms_nNumCivFemale));
	return 1;};
int get_population_max_in_use(lua_State* L) { // макс. число используемых педов
	lua_pushnumber(L, (double)(CPopulation::MaxNumberOfPedsInUse));
	return 1;};
int get_population_countdown(lua_State* L) { // обратный отсчёт до спавна педов
	lua_pushnumber(L, (double)(CPopulation::m_CountDownToPedsAtStart));
	return 1;};
int get_population_all_random_type(lua_State* L) { // тип всех случайных педов
	try {
		if (true) {
			lua_pushinteger(L, (lua_Integer)CPopulation::m_AllRandomPedsThisType);
			return 1;
		}
		else { throw "bad argument in function get_population_all_random_type"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_population_all_random_type(lua_State* L) { // уст. тип всех случайных педов
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {
			CPopulation::m_AllRandomPedsThisType = (int)lua_tointeger(L, 1);
		}
		else { throw "bad argument in function set_population_all_random_type"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_hud_m_WantedTimer(lua_State* L) { // таймер розыска на HUD
	lua_pushnumber(L, (double)(CHud::m_WantedTimer));
	return 1;};
int set_hud_m_WantedTimer(lua_State* L) { // таймер розыска на HUD
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {
			(CHud::m_WantedTimer) = (float)lua_tonumber(L, 1);
			return 0;
		}
		else { throw "bad argument in function set_hud_m_WantedTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_hud_m_WeaponFadeTimer(lua_State* L) { // таймер затухания оружия на HUD
	lua_pushnumber(L, (double)(CHud::m_WeaponFadeTimer));
	return 1;};
int set_hud_m_WeaponFadeTimer(lua_State* L) { // таймер затухания оружия на HUD
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {
			(CHud::m_WeaponFadeTimer) = (float)lua_tonumber(L, 1);
			return 0;
		}
		else { throw "bad argument in function set_hud_m_WeaponFadeTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_hud_m_EnergyLostFadeTimer(lua_State* L) { // таймер затухания потери энергии
	lua_pushnumber(L, (double)(CHud::m_EnergyLostFadeTimer));
	return 1;};
int set_hud_m_EnergyLostFadeTimer(lua_State* L) { // таймер затухания потери энергии
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {
			(CHud::m_EnergyLostFadeTimer) = (float)lua_tonumber(L, 1);
			return 0;
		}
		else { throw "bad argument in function set_hud_m_EnergyLostFadeTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_hud_m_DisplayScoreFadeTimer(lua_State* L) { // таймер затухания счёта
	lua_pushnumber(L, (double)(CHud::m_DisplayScoreFadeTimer));
	return 1;};
int set_hud_m_DisplayScoreFadeTimer(lua_State* L) { // таймер затухания счёта
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {
			(CHud::m_DisplayScoreFadeTimer) = (float)lua_tonumber(L, 1);
			return 0;
		}
		else { throw "bad argument in function set_hud_m_DisplayScoreFadeTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_hud_wanted_state(lua_State* L) { // состояние розыска на HUD
	try {
		if (true) {
			lua_pushinteger(L, (lua_Integer)CHud::m_WantedState);
			return 1;
		}
		else { throw "bad argument in function get_hud_wanted_state"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_hud_help_message_state(lua_State* L) { // состояние подсказки на HUD
	try {
		if (true) {
			lua_pushinteger(L, (lua_Integer)CHud::m_HelpMessageState);
			return 1;
		}
		else { throw "bad argument in function get_hud_help_message_state"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_player_taxi_timer(lua_State* L) { // таймер такси игрока
	try {
		if (true) {
			lua_pushinteger(L, (lua_Integer)CWorld::Players[CWorld::PlayerInFocus].m_nTaxiTimer);
			return 1;
		}
		else { throw "bad argument in function get_player_taxi_timer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_player_death_fade_timer(lua_State* L) { // таймер затухания при смерти
	try {
		if (true) {
			lua_pushinteger(L, (lua_Integer)CWorld::Players[CWorld::PlayerInFocus].m_nDeathFadeTimer);
			return 1;
		}
		else { throw "bad argument in function get_player_death_fade_timer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_player_2wheels_time(lua_State* L) { // время на двух колёсах (мс)
	try {
		if (true) {
			lua_pushinteger(L, (lua_Integer)CWorld::Players[CWorld::PlayerInFocus].m_nVehicleTimeOnTwoWheels);
			return 1;
		}
		else { throw "bad argument in function get_player_2wheels_time"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_player_2wheels_distance(lua_State* L) { // дистанция на двух колёсах
	try {
		if (true) {
			lua_pushnumber(L, CWorld::Players[CWorld::PlayerInFocus].m_fVehicleDistanceOnTwoWheels);
			return 1;
		}
		else { throw "bad argument in function get_player_2wheels_distance"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_player_road_density(lua_State* L) { // плотность дорог вокруг игрока
	try {
		if (true) {
			lua_pushnumber(L, CWorld::Players[CWorld::PlayerInFocus].m_fRoadDensityAroundPlayer);
			return 1;
		}
		else { throw "bad argument in function get_player_road_density"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_player_road_density(lua_State* L) { // уст. плотность дорог вокруг игрока
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {
			CWorld::Players[CWorld::PlayerInFocus].m_fRoadDensityAroundPlayer = (float)lua_tonumber(L, 1);
		}
		else { throw "bad argument in function set_player_road_density"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_pedik_m_fYaw(lua_State* L) { // угол наклона головы педа
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushnumber(L, ped->m_PedIK.m_sHead.m_fYaw);
			return 1;
		}
		else { throw "bad argument in function get_pedik_m_fYaw"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_pedik_m_fYaw(lua_State* L) { // уст. угол наклона головы педа
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_PedIK.m_sHead.m_fYaw = (float)lua_tonumber(L, 2);
		}
		else { throw "bad argument in function set_pedik_m_fYaw"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_pedik_m_fPitch(lua_State* L) { // наклон головы педа по тангажу
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushnumber(L, ped->m_PedIK.m_sHead.m_fPitch);
			return 1;
		}
		else { throw "bad argument in function get_pedik_m_fPitch"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_pedik_m_fPitch(lua_State* L) { // уст. наклон головы педа по тангажу
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_PedIK.m_sHead.m_fPitch = (float)lua_tonumber(L, 2);
		}
		else { throw "bad argument in function set_pedik_m_fPitch"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_boat_m_fPropRotation(lua_State* L) { // вращение винта лодки
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_BOAT) throw "not a boat";
			CBoat* boat = (CBoat*)car;
			lua_pushnumber(L, boat->m_fPropRotation);
			return 1;
		}
		else { throw "bad argument in function get_boat_m_fPropRotation"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_boat_m_fPropRotation(lua_State* L) { // уст. вращение винта лодки
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_BOAT) throw "not a boat";
			CBoat* boat = (CBoat*)car;
			boat->m_fPropRotation = (float)lua_tonumber(L, 2);
		}
		else { throw "bad argument in function set_boat_m_fPropRotation"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_boat_m_fPropSpeed(lua_State* L) { // скорость винта лодки
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_BOAT) throw "not a boat";
			CBoat* boat = (CBoat*)car;
			lua_pushnumber(L, boat->m_fPropSpeed);
			return 1;
		}
		else { throw "bad argument in function get_boat_m_fPropSpeed"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_boat_m_fPropSpeed(lua_State* L) { // уст. скорость винта лодки
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_BOAT) throw "not a boat";
			CBoat* boat = (CBoat*)car;
			boat->m_fPropSpeed = (float)lua_tonumber(L, 2);
		}
		else { throw "bad argument in function set_boat_m_fPropSpeed"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_boat_m_fForcedZRotation(lua_State* L) { // принудительный поворот лодки по Z
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_BOAT) throw "not a boat";
			CBoat* boat = (CBoat*)car;
			lua_pushnumber(L, boat->m_fForcedZRotation);
			return 1;
		}
		else { throw "bad argument in function get_boat_m_fForcedZRotation"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_boat_m_fForcedZRotation(lua_State* L) { // уст. принудительный поворот лодки по Z
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_BOAT) throw "not a boat";
			CBoat* boat = (CBoat*)car;
			boat->m_fForcedZRotation = (float)lua_tonumber(L, 2);
		}
		else { throw "bad argument in function set_boat_m_fForcedZRotation"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_boat_m_fBoatGasPedal(lua_State* L) { // газ лодки
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_BOAT) throw "not a boat";
			CBoat* boat = (CBoat*)car;
			lua_pushnumber(L, boat->m_fBoatGasPedal);
			return 1;
		}
		else { throw "bad argument in function get_boat_m_fBoatGasPedal"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_boat_m_fBoatGasPedal(lua_State* L) { // уст. газ лодки
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_BOAT) throw "not a boat";
			CBoat* boat = (CBoat*)car;
			boat->m_fBoatGasPedal = (float)lua_tonumber(L, 2);
		}
		else { throw "bad argument in function set_boat_m_fBoatGasPedal"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_boat_m_fBoatSteeringLeftRight(lua_State* L) { // руль лодки
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_BOAT) throw "not a boat";
			CBoat* boat = (CBoat*)car;
			lua_pushnumber(L, boat->m_fBoatSteeringLeftRight);
			return 1;
		}
		else { throw "bad argument in function get_boat_m_fBoatSteeringLeftRight"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_boat_m_fBoatSteeringLeftRight(lua_State* L) { // уст. руль лодки
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_BOAT) throw "not a boat";
			CBoat* boat = (CBoat*)car;
			boat->m_fBoatSteeringLeftRight = (float)lua_tonumber(L, 2);
		}
		else { throw "bad argument in function set_boat_m_fBoatSteeringLeftRight"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_bike_anim_group(lua_State* L) { // группа анимаций мотоцикла
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_BIKE) throw "not a bike";
			lua_pushinteger(L, (lua_Integer)((CBike*)car)->nBikeAnimGroup);
			return 1;
		}
		else { throw "bad argument in function get_bike_anim_group"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_bike_anim_group(lua_State* L) { // уст. группу анимаций мотоцикла
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_BIKE) throw "not a bike";
			((CBike*)car)->nBikeAnimGroup = (int)lua_tointeger(L, 2);
		}
		else { throw "bad argument in function set_bike_anim_group"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_bike_damage_flags(lua_State* L) { // флаги повреждений мотоцикла
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_BIKE) throw "not a bike";
			lua_pushinteger(L, (lua_Integer)((CBike*)car)->m_nDamageFlags);
			return 1;
		}
		else { throw "bad argument in function get_bike_damage_flags"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_bike_damage_flags(lua_State* L) { // уст. флаги повреждений мотоцикла
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_BIKE) throw "not a bike";
			((CBike*)car)->m_nDamageFlags = (unsigned char)lua_tointeger(L, 2);
		}
		else { throw "bad argument in function set_bike_damage_flags"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_time_step_non_clipped(lua_State* L) { // шаг времени без клиппинга
	try {
		if (true) {
			lua_pushnumber(L, CTimer::ms_fTimeStepNonClipped);
			return 1;
		}
		else { throw "bad argument in function get_time_step_non_clipped"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_previous_time_ms(lua_State* L) { // предыдущее время в мс
	try {
		if (true) {
			lua_pushinteger(L, (lua_Integer)CTimer::m_snPreviousTimeInMilliseconds);
			return 1;
		}
		else { throw "bad argument in function get_previous_time_ms"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_time_ms_non_clipped(lua_State* L) { // время в мс без клиппинга
	try {
		if (true) {
			lua_pushinteger(L, (lua_Integer)CTimer::m_snTimeInMillisecondsNonClipped);
			return 1;
		}
		else { throw "bad argument in function get_time_ms_non_clipped"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int is_slow_motion_active(lua_State* L) { // активен ли слоумоушен
	try {
		if (true) {
			lua_pushboolean(L, CTimer::GetIsSlowMotionActive());
			return 1;
		}
		else { throw "bad argument in function is_slow_motion_active"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_cycles_per_millisecond(lua_State* L) { // циклы на миллисекунду
	try {
		if (true) {
			lua_pushinteger(L, (lua_Integer)CTimer::GetCyclesPerMillisecond());
			return 1;
		}
		else { throw "bad argument in function get_cycles_per_millisecond"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_clock_hours(lua_State* L) { // часы игрового таймера
	try {
		if (true) {
			lua_pushinteger(L, (lua_Integer)CClock::ms_nGameClockHours);
			return 1;
		}
		else { throw "bad argument in function get_clock_hours"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_clock_minutes(lua_State* L) { // минуты игрового таймера
	try {
		if (true) {
			lua_pushinteger(L, (lua_Integer)CClock::ms_nGameClockMinutes);
			return 1;
		}
		else { throw "bad argument in function get_clock_minutes"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_clock_seconds(lua_State* L) { // секунды игрового таймера
	try {
		if (true) {
			lua_pushinteger(L, (lua_Integer)CClock::ms_nGameClockSeconds);
			return 1;
		}
		else { throw "bad argument in function get_clock_seconds"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_clock_stored_hours(lua_State* L) { // сохранённые часы
	try {
		if (true) {
			lua_pushinteger(L, (lua_Integer)CClock::ms_Stored_nGameClockHours);
			return 1;
		}
		else { throw "bad argument in function get_clock_stored_hours"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_clock_stored_minutes(lua_State* L) { // сохранённые минуты
	try {
		if (true) {
			lua_pushinteger(L, (lua_Integer)CClock::ms_Stored_nGameClockMinutes);
			return 1;
		}
		else { throw "bad argument in function get_clock_stored_minutes"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_clock_minutes_until(lua_State* L) { // минут до заданного времени
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			unsigned int r = CClock::GetGameClockMinutesUntil((unsigned char)lua_tointeger(L, 1), (unsigned char)lua_tointeger(L, 2));
			lua_pushinteger(L, (lua_Integer)r);
			return 1;
		}
		else { throw "bad argument in function get_clock_minutes_until"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int restore_clock(lua_State* L) { // восстановить сохранённое время
	CClock::RestoreClock();
	return 0;};
int get_streaming_disabled(lua_State* L) { // стриминг отключён
	try {
		if (true) {
			lua_pushboolean(L, CStreaming::ms_disableStreaming);
			return 1;
		}
		else { throw "bad argument in function get_streaming_disabled"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_streaming_disabled(lua_State* L) { // отключить стриминг моделей
	try {
		if (LUA_TBOOLEAN == lua_type(L, 1)) {
			CStreaming::ms_disableStreaming = (bool)lua_toboolean(L, 1);
		}
		else { throw "bad argument in function set_streaming_disabled"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_streaming_models_requested(lua_State* L) { // запрошено моделей
	lua_pushnumber(L, (double)(CStreaming::ms_numModelsRequested));
	return 1;};
int get_streaming_priority_requests(lua_State* L) { // приоритетных запросов
	lua_pushnumber(L, (double)(CStreaming::ms_numPriorityRequests));
	return 1;};
int get_streaming_image_size(lua_State* L) { // размер образа стриминга
	lua_pushnumber(L, (double)(CStreaming::ms_imageSize));
	return 1;};
int get_streaming_buffer_size(lua_State* L) { // размер буфера стриминга
	lua_pushnumber(L, (double)(CStreaming::ms_streamingBufferSize));
	return 1;};
int get_streaming_current_ped_grp(lua_State* L) { // текущая группа педов
	lua_pushnumber(L, (double)(CStreaming::ms_currentPedGrp));
	return 1;};
int get_streaming_loading_big_model(lua_State* L) { // загружается большая модель
	lua_pushnumber(L, (double)(CStreaming::ms_bLoadingBigModel));
	return 1;};
int get_streaming_peds_loaded(lua_State* L) { // загружено педов
	lua_pushnumber(L, (double)(CStreaming::ms_numPedsLoaded));
	return 1;};
int get_streaming_channel_error(lua_State* L) { // ошибка канала стриминга
	lua_pushnumber(L, (double)(CStreaming::ms_channelError));
	return 1;};
int get_streaming_last_image_read(lua_State* L) { // последний прочитанный образ
	lua_pushnumber(L, (double)(CStreaming::ms_lastImageRead));
	return 1;};
int get_streaming_old_sector_x(lua_State* L) { // старый сектор X
	lua_pushnumber(L, (double)(CStreaming::ms_oldSectorX));
	return 1;};
int get_streaming_old_sector_y(lua_State* L) { // старый сектор Y
	lua_pushnumber(L, (double)(CStreaming::ms_oldSectorY));
	return 1;};
int get_zone_level(lua_State* L) { // остров по координатам
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {
			CVector pos = { (float)lua_tonumber(L, 1), (float)lua_tonumber(L, 2), (float)lua_tonumber(L, 3) };
			lua_pushinteger(L, (lua_Integer)CTheZones::GetLevelFromPosition(&pos));
			return 1;
		}
		else { throw "bad argument in function get_zone_level"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int find_information_zone(lua_State* L) { // индекс информационной зоны
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {
			CVector pos = { (float)lua_tonumber(L, 1), (float)lua_tonumber(L, 2), (float)lua_tonumber(L, 3) };
			lua_pushinteger(L, (lua_Integer)CTheZones::FindInformationZoneForPosition(&pos));
			return 1;
		}
		else { throw "bad argument in function find_information_zone"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_navigation_zone(lua_State* L) { // навигационная зона по индексу
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {
			lua_pushinteger(L, (lua_Integer)CTheZones::GetNavigationZone((unsigned short)lua_tointeger(L, 1)));
			return 1;
		}
		else { throw "bad argument in function get_navigation_zone"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int init_zones(lua_State* L) { // инициализировать зоны
	CTheZones::Init();
	return 0;};
int traffic_lights_for_peds(lua_State* L) { // светофор для пешеходов
	CTrafficLights::LightForPeds();
	return 0;};
int traffic_scan_for_lights(lua_State* L) { // сканировать светофоры
	CTrafficLights::ScanForLightsOnMap();
	return 0;};
int traffic_should_car_stop_for_bridge(lua_State* L) { // машина должна остановиться у моста
	CTrafficLights::ShouldCarStopForBridge();
	return 0;};
int should_car_stop_for_light(lua_State* L) { // должна ли машина остановиться на свет
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			CTrafficLights::ShouldCarStopForLight(car, (bool)lua_toboolean(L, 2));
		}
		else { throw "bad argument in function should_car_stop_for_light"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int add_gun_flash_big(lua_State* L) { // вспышка выстрела крупного оружия
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) &&
			LUA_TNUMBER == lua_type(L, 4) && LUA_TNUMBER == lua_type(L, 5) && LUA_TNUMBER == lua_type(L, 6)) {
			CVector start = { (float)lua_tonumber(L, 1), (float)lua_tonumber(L, 2), (float)lua_tonumber(L, 3) };
			CVector end_ = { (float)lua_tonumber(L, 4), (float)lua_tonumber(L, 5), (float)lua_tonumber(L, 6) };
			CWeapon::AddGunFlashBigGuns(start, end_);
		}
		else { throw "bad argument in function add_gun_flash_big"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int initialise_weapons(lua_State* L) { // инициализировать оружие
	CWeapon::InitialiseWeapons();
	return 0;};
int shutdown_weapons(lua_State* L) { // завершить работу оружия
	CWeapon::ShutdownWeapons();
	return 0;};
int update_weapons(lua_State* L) { // обновить оружие
	CWeapon::UpdateWeapons();
	return 0;};
int do_doom_aiming(lua_State* L) { // аим-траверс для оружия
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4) &&
			LUA_TNUMBER == lua_type(L, 5) && LUA_TNUMBER == lua_type(L, 6) && LUA_TNUMBER == lua_type(L, 7)) {
			CEntity* entity = (CEntity*)lua_topointer(L, 1);
			CVector start = { (float)lua_tonumber(L, 2), (float)lua_tonumber(L, 3), (float)lua_tonumber(L, 4) };
			CVector end_ = { (float)lua_tonumber(L, 5), (float)lua_tonumber(L, 6), (float)lua_tonumber(L, 7) };
			CWeapon::DoDoomAiming(entity, &start, &end_);
		}
		else { throw "bad argument in function do_doom_aiming"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int create_some_money(lua_State* L) { // создать деньги на координатах
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {
			CVector pos = { (float)lua_tonumber(L, 1), (float)lua_tonumber(L, 2), (float)lua_tonumber(L, 3) };
			CPickups::CreateSomeMoney(pos, (int)lua_tointeger(L, 4));
		}
		else { throw "bad argument in function create_some_money"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int give_player_goodies(lua_State* L) { // выдать игроку бонусы по модели
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			bool r = CPickups::GivePlayerGoodiesWithPickUpMI((unsigned short)lua_tointeger(L, 1), (int)lua_tointeger(L, 2));
			lua_pushboolean(L, r);
			return 1;
		}
		else { throw "bad argument in function give_player_goodies"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int do_pickup_effects(lua_State* L) { // эффект поднятия пикапа
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPickups::DoPickUpEffects((CEntity*)lua_topointer(L, 1));
		}
		else { throw "bad argument in function do_pickup_effects"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int do_money_effects(lua_State* L) { // эффект денег
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPickups::DoMoneyEffects((CEntity*)lua_topointer(L, 1));
		}
		else { throw "bad argument in function do_money_effects"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int do_mine_effects(lua_State* L) { // эффект мины
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPickups::DoMineEffects((CEntity*)lua_topointer(L, 1));
		}
		else { throw "bad argument in function do_mine_effects"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_obj_m_fAttachForce(lua_State* L) { // сила привязки объекта
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CObject* obj = findobjinpool(lua_topointer(L, 1));
			lua_pushnumber(L, (float)obj->m_fAttachForce);
			return 1;
		}
		else { throw "bad argument in function get_obj_m_fAttachForce"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_obj_m_fAttachForce(lua_State* L) { // уст. сила привязки объекта
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CObject* obj = findobjinpool(lua_topointer(L, 1));
			obj->m_fAttachForce = (float)lua_tonumber(L, 2);
		}
		else { throw "bad argument in function set_obj_m_fAttachForce"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_obj_m_CollisionDamageType(lua_State* L) { // тип урона от столкновений
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CObject* obj = findobjinpool(lua_topointer(L, 1));
			lua_pushnumber(L, (int)obj->m_CollisionDamageType);
			return 1;
		}
		else { throw "bad argument in function get_obj_m_CollisionDamageType"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_obj_m_CollisionDamageType(lua_State* L) { // уст. тип урона от столкновений
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CObject* obj = findobjinpool(lua_topointer(L, 1));
			obj->m_CollisionDamageType = (unsigned char)lua_tonumber(L, 2);
		}
		else { throw "bad argument in function set_obj_m_CollisionDamageType"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_obj_m_nSpecialCollisionType(lua_State* L) { // специальный тип столкновений
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CObject* obj = findobjinpool(lua_topointer(L, 1));
			lua_pushnumber(L, (int)obj->m_nSpecialCollisionType);
			return 1;
		}
		else { throw "bad argument in function get_obj_m_nSpecialCollisionType"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_obj_m_nSpecialCollisionType(lua_State* L) { // уст. специальный тип столкновений
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CObject* obj = findobjinpool(lua_topointer(L, 1));
			obj->m_nSpecialCollisionType = (unsigned char)lua_tonumber(L, 2);
		}
		else { throw "bad argument in function set_obj_m_nSpecialCollisionType"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_obj_m_nBounceScore(lua_State* L) { // счёт отскока объекта
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CObject* obj = findobjinpool(lua_topointer(L, 1));
			lua_pushnumber(L, (int)obj->m_nBounceScore);
			return 1;
		}
		else { throw "bad argument in function get_obj_m_nBounceScore"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_obj_m_nBounceScore(lua_State* L) { // уст. счёт отскока объекта
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CObject* obj = findobjinpool(lua_topointer(L, 1));
			obj->m_nBounceScore = (unsigned char)lua_tonumber(L, 2);
		}
		else { throw "bad argument in function set_obj_m_nBounceScore"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_obj_m_dwObjectTimer(lua_State* L) { // таймер объекта
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CObject* obj = findobjinpool(lua_topointer(L, 1));
			lua_pushnumber(L, (int)obj->m_dwObjectTimer);
			return 1;
		}
		else { throw "bad argument in function get_obj_m_dwObjectTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_obj_m_dwObjectTimer(lua_State* L) { // уст. таймер объекта
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CObject* obj = findobjinpool(lua_topointer(L, 1));
			obj->m_dwObjectTimer = (int)lua_tonumber(L, 2);
		}
		else { throw "bad argument in function set_obj_m_dwObjectTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_radar_range(lua_State* L) { // дальность радара
	try {
		if (true) {
			lua_pushnumber(L, CRadar::m_radarRange);
			return 1;
		}
		else { throw "bad argument in function get_radar_range"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_radar_range(lua_State* L) { // уст. дальность радара
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {
			CRadar::m_radarRange = (float)lua_tonumber(L, 1);
		}
		else { throw "bad argument in function set_radar_range"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_sun_blocked_by_clouds(lua_State* L) { // уст. солнце закрыто облаками
	try {
		if (LUA_TBOOLEAN == lua_type(L, 1)) {
			CCoronas::SunBlockedByClouds = (bool)lua_toboolean(L, 1);
		}
		else { throw "bad argument in function set_sun_blocked_by_clouds"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int update_corona_coors(lua_State* L) { // обновить координаты короны
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4) && LUA_TNUMBER == lua_type(L, 5) && LUA_TNUMBER == lua_type(L, 6)) {
			CVector pos = { (float)lua_tonumber(L, 2), (float)lua_tonumber(L, 3), (float)lua_tonumber(L, 4) };
			CCoronas::UpdateCoronaCoors((unsigned int)lua_tointeger(L, 1), pos, (float)lua_tonumber(L, 5), (float)lua_tonumber(L, 6));
		}
		else { throw "bad argument in function update_corona_coors"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int is_cutscene_running(lua_State* L) { // катсцена идёт
	try {
		if (true) {
			lua_pushboolean(L, CCutsceneMgr::ms_running);
			return 1;
		}
		else { throw "bad argument in function is_cutscene_running"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_num_cutscene_objs(lua_State* L) { // число объектов катсцены
	try {
		if (true) {
			lua_pushinteger(L, (lua_Integer)CCutsceneMgr::ms_numCutsceneObjs);
			return 1;
		}
		else { throw "bad argument in function get_num_cutscene_objs"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_cutscene_time_ms(lua_State* L) { // время катсцены в мс
	try {
		if (true) {
			lua_pushinteger(L, (lua_Integer)CCutsceneMgr::GetCutsceneTimeInMilliseconds());
			return 1;
		}
		else { throw "bad argument in function get_cutscene_time_ms"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int choose_police_car_model(lua_State* L) { // выбрать модель полицейской машины
	try {
		if (true) {
			lua_pushinteger(L, (lua_Integer)CCarCtrl::ChoosePoliceCarModel());
			return 1;
		}
		else { throw "bad argument in function choose_police_car_model"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int generate_emergency_services(lua_State* L) { // вызвать экстренные службы
	CCarCtrl::GenerateEmergencyServicesCar();
	return 0;};
int clear_interesting_vehicles(lua_State* L) { // очистить список интересных машин
	CCarCtrl::ClearInterestingVehicleList();
	return 0;};
int drag_car_to_point(lua_State* L) { // тащить машину к точке
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			CVector pos = { (float)lua_tonumber(L, 2), (float)lua_tonumber(L, 3), (float)lua_tonumber(L, 4) };
			CCarCtrl::DragCarToPoint(car, &pos);
		}
		else { throw "bad argument in function drag_car_to_point"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int find_max_speed_in_traffic(lua_State* L) { // макс. скорость машины в трафике
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			lua_pushnumber(L, CCarCtrl::FindMaximumSpeedForThisCarInTraffic(car));
			return 1;
		}
		else { throw "bad argument in function find_max_speed_in_traffic"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_num_bright_lights(lua_State* L) { // число ярких огней
	try {
		if (true) {
			lua_pushinteger(L, (lua_Integer)CBrightLights::NumBrightLights);
			return 1;
		}
		else { throw "bad argument in function get_num_bright_lights"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_nPrimaryColor(lua_State* L) { // m_nPrimaryColor (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			lua_pushinteger(L, (lua_Integer)car->m_nPrimaryColor);
			return 1;
		}
		else { throw "bad argument in function get_car_m_nPrimaryColor"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_nPrimaryColor(lua_State* L) { // уст. m_nPrimaryColor (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			car->m_nPrimaryColor = (unsigned char)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_nPrimaryColor"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_nSecondaryColor(lua_State* L) { // m_nSecondaryColor (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			lua_pushinteger(L, (lua_Integer)car->m_nSecondaryColor);
			return 1;
		}
		else { throw "bad argument in function get_car_m_nSecondaryColor"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_nSecondaryColor(lua_State* L) { // уст. m_nSecondaryColor (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			car->m_nSecondaryColor = (unsigned char)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_nSecondaryColor"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_wWantedStarsOnEnter(lua_State* L) { // m_wWantedStarsOnEnter (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			lua_pushinteger(L, (lua_Integer)car->m_wWantedStarsOnEnter);
			return 1;
		}
		else { throw "bad argument in function get_car_m_wWantedStarsOnEnter"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_wWantedStarsOnEnter(lua_State* L) { // уст. m_wWantedStarsOnEnter (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			car->m_wWantedStarsOnEnter = (short)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_wWantedStarsOnEnter"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_wMissionValue(lua_State* L) { // m_wMissionValue (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			lua_pushinteger(L, (lua_Integer)car->m_wMissionValue);
			return 1;
		}
		else { throw "bad argument in function get_car_m_wMissionValue"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_wMissionValue(lua_State* L) { // уст. m_wMissionValue (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			car->m_wMissionValue = (short)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_wMissionValue"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_nNumPassengers(lua_State* L) { // m_nNumPassengers (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			lua_pushinteger(L, (lua_Integer)car->m_nNumPassengers);
			return 1;
		}
		else { throw "bad argument in function get_car_m_nNumPassengers"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_nNumPassengers(lua_State* L) { // уст. m_nNumPassengers (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			car->m_nNumPassengers = (unsigned char)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_nNumPassengers"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_nNumGettingIn(lua_State* L) { // m_nNumGettingIn (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			lua_pushinteger(L, (lua_Integer)car->m_nNumGettingIn);
			return 1;
		}
		else { throw "bad argument in function get_car_m_nNumGettingIn"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_nNumGettingIn(lua_State* L) { // уст. m_nNumGettingIn (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			car->m_nNumGettingIn = (unsigned char)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_nNumGettingIn"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_nGettingInFlags(lua_State* L) { // m_nGettingInFlags (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			lua_pushinteger(L, (lua_Integer)car->m_nGettingInFlags);
			return 1;
		}
		else { throw "bad argument in function get_car_m_nGettingInFlags"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_nGettingInFlags(lua_State* L) { // уст. m_nGettingInFlags (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			car->m_nGettingInFlags = (unsigned char)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_nGettingInFlags"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_nGettingOutFlags(lua_State* L) { // m_nGettingOutFlags (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			lua_pushinteger(L, (lua_Integer)car->m_nGettingOutFlags);
			return 1;
		}
		else { throw "bad argument in function get_car_m_nGettingOutFlags"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_nGettingOutFlags(lua_State* L) { // уст. m_nGettingOutFlags (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			car->m_nGettingOutFlags = (unsigned char)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_nGettingOutFlags"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_nMaxPassengers(lua_State* L) { // m_nMaxPassengers (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			lua_pushinteger(L, (lua_Integer)car->m_nMaxPassengers);
			return 1;
		}
		else { throw "bad argument in function get_car_m_nMaxPassengers"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_nMaxPassengers(lua_State* L) { // уст. m_nMaxPassengers (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			car->m_nMaxPassengers = (unsigned char)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_nMaxPassengers"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_fSteerAngle(lua_State* L) { // m_fSteerAngle (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			lua_pushnumber(L, car->m_fSteerAngle);
			return 1;
		}
		else { throw "bad argument in function get_car_m_fSteerAngle"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_fSteerAngle(lua_State* L) { // уст. m_fSteerAngle (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			car->m_fSteerAngle = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_fSteerAngle"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_fGasPedal(lua_State* L) { // m_fGasPedal (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			lua_pushnumber(L, car->m_fGasPedal);
			return 1;
		}
		else { throw "bad argument in function get_car_m_fGasPedal"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_fGasPedal(lua_State* L) { // уст. m_fGasPedal (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			car->m_fGasPedal = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_fGasPedal"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_fBreakPedal(lua_State* L) { // m_fBreakPedal (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			lua_pushnumber(L, car->m_fBreakPedal);
			return 1;
		}
		else { throw "bad argument in function get_car_m_fBreakPedal"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_fBreakPedal(lua_State* L) { // уст. m_fBreakPedal (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			car->m_fBreakPedal = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_fBreakPedal"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_nCreatedBy(lua_State* L) { // m_nCreatedBy (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			lua_pushinteger(L, (lua_Integer)car->m_nCreatedBy);
			return 1;
		}
		else { throw "bad argument in function get_car_m_nCreatedBy"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_nCreatedBy(lua_State* L) { // уст. m_nCreatedBy (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			car->m_nCreatedBy = (unsigned char)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_nCreatedBy"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_nAmmoInClip(lua_State* L) { // m_nAmmoInClip (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			lua_pushinteger(L, (lua_Integer)car->m_nAmmoInClip);
			return 1;
		}
		else { throw "bad argument in function get_car_m_nAmmoInClip"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_nAmmoInClip(lua_State* L) { // уст. m_nAmmoInClip (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			car->m_nAmmoInClip = (unsigned char)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_nAmmoInClip"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_fHealth(lua_State* L) { // m_fHealth (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			lua_pushnumber(L, car->m_fHealth);
			return 1;
		}
		else { throw "bad argument in function get_car_m_fHealth"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_fHealth(lua_State* L) { // уст. m_fHealth (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			car->m_fHealth = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_fHealth"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_nCurrentGear(lua_State* L) { // m_nCurrentGear (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			lua_pushinteger(L, (lua_Integer)car->m_nCurrentGear);
			return 1;
		}
		else { throw "bad argument in function get_car_m_nCurrentGear"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_nCurrentGear(lua_State* L) { // уст. m_nCurrentGear (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			car->m_nCurrentGear = (unsigned char)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_nCurrentGear"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_nTimeTillWeNeedThisCar(lua_State* L) { // m_nTimeTillWeNeedThisCar (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			lua_pushinteger(L, (lua_Integer)car->m_nTimeTillWeNeedThisCar);
			return 1;
		}
		else { throw "bad argument in function get_car_m_nTimeTillWeNeedThisCar"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_nTimeTillWeNeedThisCar(lua_State* L) { // уст. m_nTimeTillWeNeedThisCar (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			car->m_nTimeTillWeNeedThisCar = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_nTimeTillWeNeedThisCar"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_nTimeOfDeath(lua_State* L) { // m_nTimeOfDeath (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			lua_pushinteger(L, (lua_Integer)car->m_nTimeOfDeath);
			return 1;
		}
		else { throw "bad argument in function get_car_m_nTimeOfDeath"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_nTimeOfDeath(lua_State* L) { // уст. m_nTimeOfDeath (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			car->m_nTimeOfDeath = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_nTimeOfDeath"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_wBombTimer(lua_State* L) { // m_wBombTimer (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			lua_pushinteger(L, (lua_Integer)car->m_wBombTimer);
			return 1;
		}
		else { throw "bad argument in function get_car_m_wBombTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_wBombTimer(lua_State* L) { // уст. m_wBombTimer (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			car->m_wBombTimer = (short)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_wBombTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_nLastWeaponDamage(lua_State* L) { // m_nLastWeaponDamage (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			lua_pushinteger(L, (lua_Integer)car->m_nLastWeaponDamage);
			return 1;
		}
		else { throw "bad argument in function get_car_m_nLastWeaponDamage"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_nLastWeaponDamage(lua_State* L) { // уст. m_nLastWeaponDamage (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			car->m_nLastWeaponDamage = (unsigned char)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_nLastWeaponDamage"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_nRadioStation(lua_State* L) { // m_nRadioStation (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			lua_pushinteger(L, (lua_Integer)car->m_nRadioStation);
			return 1;
		}
		else { throw "bad argument in function get_car_m_nRadioStation"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_nRadioStation(lua_State* L) { // уст. m_nRadioStation (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			car->m_nRadioStation = (unsigned char)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_nRadioStation"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_bHornEnabled(lua_State* L) { // m_bHornEnabled (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			lua_pushinteger(L, (lua_Integer)car->m_bHornEnabled);
			return 1;
		}
		else { throw "bad argument in function get_car_m_bHornEnabled"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_bHornEnabled(lua_State* L) { // уст. m_bHornEnabled (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			car->m_bHornEnabled = (unsigned int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_bHornEnabled"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_nSirenOrAlarm(lua_State* L) { // m_nSirenOrAlarm (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			lua_pushinteger(L, (lua_Integer)car->m_nSirenOrAlarm);
			return 1;
		}
		else { throw "bad argument in function get_car_m_nSirenOrAlarm"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_nSirenOrAlarm(lua_State* L) { // уст. m_nSirenOrAlarm (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			car->m_nSirenOrAlarm = (unsigned char)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_nSirenOrAlarm"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_nSirenExtra(lua_State* L) { // m_nSirenExtra (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			lua_pushinteger(L, (lua_Integer)car->m_nSirenExtra);
			return 1;
		}
		else { throw "bad argument in function get_car_m_nSirenExtra"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_nSirenExtra(lua_State* L) { // уст. m_nSirenExtra (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			car->m_nSirenExtra = (unsigned char)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_nSirenExtra"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_fSteerRatio(lua_State* L) { // m_fSteerRatio (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			lua_pushnumber(L, car->m_fSteerRatio);
			return 1;
		}
		else { throw "bad argument in function get_car_m_fSteerRatio"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_fSteerRatio(lua_State* L) { // уст. m_fSteerRatio (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			car->m_fSteerRatio = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_fSteerRatio"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_nVehicleClass(lua_State* L) { // m_nVehicleClass (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			lua_pushinteger(L, (lua_Integer)car->m_nVehicleClass);
			return 1;
		}
		else { throw "bad argument in function get_car_m_nVehicleClass"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_nVehicleClass(lua_State* L) { // уст. m_nVehicleClass (CVehicle)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			car;
			car->m_nVehicleClass = (unsigned int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_nVehicleClass"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_fCollisionSpeed(lua_State* L) { // m_fCollisionSpeed (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushnumber(L, ped->m_fCollisionSpeed);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_fCollisionSpeed"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_fCollisionSpeed(lua_State* L) { // уст. m_fCollisionSpeed (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_fCollisionSpeed = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_fCollisionSpeed"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nGangFlags(lua_State* L) { // m_nGangFlags (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nGangFlags);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nGangFlags"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nGangFlags(lua_State* L) { // уст. m_nGangFlags (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nGangFlags = (unsigned char)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nGangFlags"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nPedStatus(lua_State* L) { // m_nPedStatus (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nPedStatus);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nPedStatus"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nPedStatus(lua_State* L) { // уст. m_nPedStatus (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nPedStatus = (unsigned char)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nPedStatus"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_vecObjective(lua_State* L) { // m_vecObjective (CPed) (x,y,z)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushnumber(L, ped->m_vecObjective.x);
			lua_pushnumber(L, ped->m_vecObjective.y);
			lua_pushnumber(L, ped->m_vecObjective.z);
			return 3;
		}
		else { throw "bad argument in function get_ped_m_vecObjective"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_vecObjective(lua_State* L) { // уст. m_vecObjective (CPed) (x,y,z)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_vecObjective.x = (float)lua_tonumber(L, 2);
			ped->m_vecObjective.y = (float)lua_tonumber(L, 3);
			ped->m_vecObjective.z = (float)lua_tonumber(L, 4);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_vecObjective"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_fObjectiveAngle(lua_State* L) { // m_fObjectiveAngle (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushnumber(L, ped->m_fObjectiveAngle);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_fObjectiveAngle"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_fObjectiveAngle(lua_State* L) { // уст. m_fObjectiveAngle (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_fObjectiveAngle = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_fObjectiveAngle"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nPedFormation(lua_State* L) { // m_nPedFormation (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nPedFormation);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nPedFormation"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nPedFormation(lua_State* L) { // уст. m_nPedFormation (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nPedFormation = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nPedFormation"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nFearFlags(lua_State* L) { // m_nFearFlags (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nFearFlags);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nFearFlags"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nFearFlags(lua_State* L) { // уст. m_nFearFlags (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nFearFlags = (unsigned int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nFearFlags"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nEventType(lua_State* L) { // m_nEventType (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nEventType);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nEventType"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nEventType(lua_State* L) { // уст. m_nEventType (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nEventType = (unsigned int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nEventType"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_fAngleToEvent(lua_State* L) { // m_fAngleToEvent (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushnumber(L, ped->m_fAngleToEvent);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_fAngleToEvent"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_fAngleToEvent(lua_State* L) { // уст. m_fAngleToEvent (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_fAngleToEvent = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_fAngleToEvent"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nAnimGroupId(lua_State* L) { // m_nAnimGroupId (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nAnimGroupId);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nAnimGroupId"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nAnimGroupId(lua_State* L) { // уст. m_nAnimGroupId (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nAnimGroupId = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nAnimGroupId"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_vecOffsetSeek(lua_State* L) { // m_vecOffsetSeek (CPed) (x,y,z)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushnumber(L, ped->m_vecOffsetSeek.x);
			lua_pushnumber(L, ped->m_vecOffsetSeek.y);
			lua_pushnumber(L, ped->m_vecOffsetSeek.z);
			return 3;
		}
		else { throw "bad argument in function get_ped_m_vecOffsetSeek"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_vecOffsetSeek(lua_State* L) { // уст. m_vecOffsetSeek (CPed) (x,y,z)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_vecOffsetSeek.x = (float)lua_tonumber(L, 2);
			ped->m_vecOffsetSeek.y = (float)lua_tonumber(L, 3);
			ped->m_vecOffsetSeek.z = (float)lua_tonumber(L, 4);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_vecOffsetSeek"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nActionTimer(lua_State* L) { // m_nActionTimer (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nActionTimer);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nActionTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nActionTimer(lua_State* L) { // уст. m_nActionTimer (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nActionTimer = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nActionTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nWaitState(lua_State* L) { // m_nWaitState (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nWaitState);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nWaitState"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nWaitState(lua_State* L) { // уст. m_nWaitState (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nWaitState = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nWaitState"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nWaitTimer(lua_State* L) { // m_nWaitTimer (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nWaitTimer);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nWaitTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nWaitTimer(lua_State* L) { // уст. m_nWaitTimer (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nWaitTimer = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nWaitTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_wPathNodes(lua_State* L) { // m_wPathNodes (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_wPathNodes);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_wPathNodes"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_wPathNodes(lua_State* L) { // уст. m_wPathNodes (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_wPathNodes = (unsigned short)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_wPathNodes"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_wCurPathNode(lua_State* L) { // m_wCurPathNode (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_wCurPathNode);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_wCurPathNode"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_wCurPathNode(lua_State* L) { // уст. m_wCurPathNode (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_wCurPathNode = (short)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_wCurPathNode"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nPathNodeTimer(lua_State* L) { // m_nPathNodeTimer (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nPathNodeTimer);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nPathNodeTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nPathNodeTimer(lua_State* L) { // уст. m_nPathNodeTimer (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nPathNodeTimer = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nPathNodeTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_vecPathNextNode(lua_State* L) { // m_vecPathNextNode (CPed) (x,y,z)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushnumber(L, ped->m_vecPathNextNode.x);
			lua_pushnumber(L, ped->m_vecPathNextNode.y);
			lua_pushnumber(L, ped->m_vecPathNextNode.z);
			return 3;
		}
		else { throw "bad argument in function get_ped_m_vecPathNextNode"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_vecPathNextNode(lua_State* L) { // уст. m_vecPathNextNode (CPed) (x,y,z)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_vecPathNextNode.x = (float)lua_tonumber(L, 2);
			ped->m_vecPathNextNode.y = (float)lua_tonumber(L, 3);
			ped->m_vecPathNextNode.z = (float)lua_tonumber(L, 4);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_vecPathNextNode"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_fPathNextNodeDir(lua_State* L) { // m_fPathNextNodeDir (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushnumber(L, ped->m_fPathNextNodeDir);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_fPathNextNodeDir"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_fPathNextNodeDir(lua_State* L) { // уст. m_fPathNextNodeDir (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_fPathNextNodeDir = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_fPathNextNodeDir"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nPathNodeType(lua_State* L) { // m_nPathNodeType (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nPathNodeType);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nPathNodeType"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nPathNodeType(lua_State* L) { // уст. m_nPathNodeType (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nPathNodeType = (unsigned int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nPathNodeType"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_fHealth(lua_State* L) { // m_fHealth (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushnumber(L, ped->m_fHealth);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_fHealth"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_fHealth(lua_State* L) { // уст. m_fHealth (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_fHealth = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_fHealth"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_fArmour(lua_State* L) { // m_fArmour (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushnumber(L, ped->m_fArmour);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_fArmour"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_fArmour(lua_State* L) { // уст. m_fArmour (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_fArmour = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_fArmour"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nShadowUpdateTimer(lua_State* L) { // m_nShadowUpdateTimer (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nShadowUpdateTimer);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nShadowUpdateTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nShadowUpdateTimer(lua_State* L) { // уст. m_nShadowUpdateTimer (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nShadowUpdateTimer = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nShadowUpdateTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_wRouteLastPoint(lua_State* L) { // wRouteLastPoint (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->wRouteLastPoint);
			return 1;
		}
		else { throw "bad argument in function get_ped_wRouteLastPoint"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_wRouteLastPoint(lua_State* L) { // уст. wRouteLastPoint (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->wRouteLastPoint = (short)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_wRouteLastPoint"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_wRoutePoints(lua_State* L) { // wRoutePoints (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->wRoutePoints);
			return 1;
		}
		else { throw "bad argument in function get_ped_wRoutePoints"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_wRoutePoints(lua_State* L) { // уст. wRoutePoints (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->wRoutePoints = (unsigned short)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_wRoutePoints"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_wRoutePos(lua_State* L) { // wRoutePos (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->wRoutePos);
			return 1;
		}
		else { throw "bad argument in function get_ped_wRoutePos"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_wRoutePos(lua_State* L) { // уст. wRoutePos (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->wRoutePos = (short)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_wRoutePos"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_wRouteType(lua_State* L) { // wRouteType (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->wRouteType);
			return 1;
		}
		else { throw "bad argument in function get_ped_wRouteType"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_wRouteType(lua_State* L) { // уст. wRouteType (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->wRouteType = (unsigned short)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_wRouteType"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_wRouteCurDir(lua_State* L) { // wRouteCurDir (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->wRouteCurDir);
			return 1;
		}
		else { throw "bad argument in function get_ped_wRouteCurDir"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_wRouteCurDir(lua_State* L) { // уст. wRouteCurDir (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->wRouteCurDir = (short)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_wRouteCurDir"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_fHeadingCurrent(lua_State* L) { // m_fHeadingCurrent (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushnumber(L, ped->m_fHeadingCurrent);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_fHeadingCurrent"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_fHeadingCurrent(lua_State* L) { // уст. m_fHeadingCurrent (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_fHeadingCurrent = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_fHeadingCurrent"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_fHeadingGoal(lua_State* L) { // m_fHeadingGoal (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushnumber(L, ped->m_fHeadingGoal);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_fHeadingGoal"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_fHeadingGoal(lua_State* L) { // уст. m_fHeadingGoal (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_fHeadingGoal = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_fHeadingGoal"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_fHeadingChangeRate(lua_State* L) { // m_fHeadingChangeRate (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushnumber(L, ped->m_fHeadingChangeRate);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_fHeadingChangeRate"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_fHeadingChangeRate(lua_State* L) { // уст. m_fHeadingChangeRate (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_fHeadingChangeRate = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_fHeadingChangeRate"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nEnterType(lua_State* L) { // m_nEnterType (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nEnterType);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nEnterType"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nEnterType(lua_State* L) { // уст. m_nEnterType (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nEnterType = (unsigned short)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nEnterType"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nWalkAroundType(lua_State* L) { // m_nWalkAroundType (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nWalkAroundType);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nWalkAroundType"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nWalkAroundType(lua_State* L) { // уст. m_nWalkAroundType (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nWalkAroundType = (unsigned short)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nWalkAroundType"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_vecOffsetFromPhysSurface(lua_State* L) { // m_vecOffsetFromPhysSurface (CPed) (x,y,z)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushnumber(L, ped->m_vecOffsetFromPhysSurface.x);
			lua_pushnumber(L, ped->m_vecOffsetFromPhysSurface.y);
			lua_pushnumber(L, ped->m_vecOffsetFromPhysSurface.z);
			return 3;
		}
		else { throw "bad argument in function get_ped_m_vecOffsetFromPhysSurface"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_vecOffsetFromPhysSurface(lua_State* L) { // уст. m_vecOffsetFromPhysSurface (CPed) (x,y,z)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_vecOffsetFromPhysSurface.x = (float)lua_tonumber(L, 2);
			ped->m_vecOffsetFromPhysSurface.y = (float)lua_tonumber(L, 3);
			ped->m_vecOffsetFromPhysSurface.z = (float)lua_tonumber(L, 4);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_vecOffsetFromPhysSurface"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_vecSeekVehicle(lua_State* L) { // vecSeekVehicle (CPed) (x,y,z)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushnumber(L, ped->vecSeekVehicle.x);
			lua_pushnumber(L, ped->vecSeekVehicle.y);
			lua_pushnumber(L, ped->vecSeekVehicle.z);
			return 3;
		}
		else { throw "bad argument in function get_ped_vecSeekVehicle"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_vecSeekVehicle(lua_State* L) { // уст. vecSeekVehicle (CPed) (x,y,z)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->vecSeekVehicle.x = (float)lua_tonumber(L, 2);
			ped->vecSeekVehicle.y = (float)lua_tonumber(L, 3);
			ped->vecSeekVehicle.z = (float)lua_tonumber(L, 4);
			return 0;
		}
		else { throw "bad argument in function set_ped_vecSeekVehicle"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_bInVehicle(lua_State* L) { // m_bInVehicle (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushboolean(L, ped->m_bInVehicle);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_bInVehicle"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_bInVehicle(lua_State* L) { // уст. m_bInVehicle (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_bInVehicle = (bool)lua_toboolean(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_bInVehicle"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_fSeatPrecisionX(lua_State* L) { // m_fSeatPrecisionX (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushnumber(L, ped->m_fSeatPrecisionX);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_fSeatPrecisionX"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_fSeatPrecisionX(lua_State* L) { // уст. m_fSeatPrecisionX (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_fSeatPrecisionX = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_fSeatPrecisionX"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_fSeatPrecisionY(lua_State* L) { // m_fSeatPrecisionY (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushnumber(L, ped->m_fSeatPrecisionY);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_fSeatPrecisionY"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_fSeatPrecisionY(lua_State* L) { // уст. m_fSeatPrecisionY (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_fSeatPrecisionY = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_fSeatPrecisionY"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nSeatType(lua_State* L) { // m_nSeatType (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nSeatType);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nSeatType"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nSeatType(lua_State* L) { // уст. m_nSeatType (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nSeatType = (unsigned int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nSeatType"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_bHasPhone(lua_State* L) { // m_bHasPhone (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushboolean(L, ped->m_bHasPhone);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_bHasPhone"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_bHasPhone(lua_State* L) { // уст. m_bHasPhone (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_bHasPhone = (bool)lua_toboolean(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_bHasPhone"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_wPhoneId(lua_State* L) { // m_wPhoneId (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_wPhoneId);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_wPhoneId"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_wPhoneId(lua_State* L) { // уст. m_wPhoneId (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_wPhoneId = (short)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_wPhoneId"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nLookingForPhone(lua_State* L) { // m_nLookingForPhone (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nLookingForPhone);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nLookingForPhone"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nLookingForPhone(lua_State* L) { // уст. m_nLookingForPhone (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nLookingForPhone = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nLookingForPhone"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nPhoneTalkTimer(lua_State* L) { // m_nPhoneTalkTimer (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nPhoneTalkTimer);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nPhoneTalkTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nPhoneTalkTimer(lua_State* L) { // уст. m_nPhoneTalkTimer (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nPhoneTalkTimer = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nPhoneTalkTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_fFleeFromPosX(lua_State* L) { // m_fFleeFromPosX (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushnumber(L, ped->m_fFleeFromPosX);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_fFleeFromPosX"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_fFleeFromPosX(lua_State* L) { // уст. m_fFleeFromPosX (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_fFleeFromPosX = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_fFleeFromPosX"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_fFleeFromPosY(lua_State* L) { // m_fFleeFromPosY (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushnumber(L, ped->m_fFleeFromPosY);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_fFleeFromPosY"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_fFleeFromPosY(lua_State* L) { // уст. m_fFleeFromPosY (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_fFleeFromPosY = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_fFleeFromPosY"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nFleeTimer(lua_State* L) { // m_nFleeTimer (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nFleeTimer);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nFleeTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nFleeTimer(lua_State* L) { // уст. m_nFleeTimer (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nFleeTimer = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nFleeTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nLastThreatTimer(lua_State* L) { // m_nLastThreatTimer (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nLastThreatTimer);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nLastThreatTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nLastThreatTimer(lua_State* L) { // уст. m_nLastThreatTimer (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nLastThreatTimer = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nLastThreatTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nStateUnused(lua_State* L) { // m_nStateUnused (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nStateUnused);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nStateUnused"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nStateUnused(lua_State* L) { // уст. m_nStateUnused (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nStateUnused = (unsigned char)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nStateUnused"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nTimerUnused(lua_State* L) { // m_nTimerUnused (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nTimerUnused);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nTimerUnused"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nTimerUnused(lua_State* L) { // уст. m_nTimerUnused (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nTimerUnused = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nTimerUnused"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nAtchStoredWep(lua_State* L) { // m_nAtchStoredWep (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nAtchStoredWep);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nAtchStoredWep"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nAtchStoredWep(lua_State* L) { // уст. m_nAtchStoredWep (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nAtchStoredWep = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nAtchStoredWep"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nStoredGiveWep(lua_State* L) { // m_nStoredGiveWep (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nStoredGiveWep);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nStoredGiveWep"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nStoredGiveWep(lua_State* L) { // уст. m_nStoredGiveWep (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nStoredGiveWep = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nStoredGiveWep"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nStoredGiveAmmo(lua_State* L) { // m_nStoredGiveAmmo (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nStoredGiveAmmo);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nStoredGiveAmmo"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nStoredGiveAmmo(lua_State* L) { // уст. m_nStoredGiveAmmo (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nStoredGiveAmmo = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nStoredGiveAmmo"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nCurrentWeapon(lua_State* L) { // m_nCurrentWeapon (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nCurrentWeapon);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nCurrentWeapon"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nCurrentWeapon(lua_State* L) { // уст. m_nCurrentWeapon (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nCurrentWeapon = (unsigned char)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nCurrentWeapon"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nWepSkills(lua_State* L) { // m_nWepSkills (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nWepSkills);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nWepSkills"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nWepSkills(lua_State* L) { // уст. m_nWepSkills (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nWepSkills = (unsigned char)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nWepSkills"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nWeaponAccuracy(lua_State* L) { // m_nWeaponAccuracy (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nWeaponAccuracy);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nWeaponAccuracy"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nWeaponAccuracy(lua_State* L) { // уст. m_nWeaponAccuracy (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nWeaponAccuracy = (unsigned char)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nWeaponAccuracy"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nBodyPart(lua_State* L) { // m_nBodyPart (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nBodyPart);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nBodyPart"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nBodyPart(lua_State* L) { // уст. m_nBodyPart (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nBodyPart = (unsigned char)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nBodyPart"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_vecHitLastPos(lua_State* L) { // m_vecHitLastPos (CPed) (x,y,z)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushnumber(L, ped->m_vecHitLastPos.x);
			lua_pushnumber(L, ped->m_vecHitLastPos.y);
			lua_pushnumber(L, ped->m_vecHitLastPos.z);
			return 3;
		}
		else { throw "bad argument in function get_ped_m_vecHitLastPos"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_vecHitLastPos(lua_State* L) { // уст. m_vecHitLastPos (CPed) (x,y,z)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_vecHitLastPos.x = (float)lua_tonumber(L, 2);
			ped->m_vecHitLastPos.y = (float)lua_tonumber(L, 3);
			ped->m_vecHitLastPos.z = (float)lua_tonumber(L, 4);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_vecHitLastPos"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nHitCounter(lua_State* L) { // m_nHitCounter (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nHitCounter);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nHitCounter"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nHitCounter(lua_State* L) { // уст. m_nHitCounter (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nHitCounter = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nHitCounter"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nLastHitState(lua_State* L) { // m_nLastHitState (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nLastHitState);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nLastHitState"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nLastHitState(lua_State* L) { // уст. m_nLastHitState (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nLastHitState = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nLastHitState"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nFightFlags1(lua_State* L) { // m_nFightFlags1 (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nFightFlags1);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nFightFlags1"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nFightFlags1(lua_State* L) { // уст. m_nFightFlags1 (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nFightFlags1 = (unsigned char)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nFightFlags1"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nFightFlags2(lua_State* L) { // m_nFightFlags2 (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nFightFlags2);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nFightFlags2"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nFightFlags2(lua_State* L) { // уст. m_nFightFlags2 (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nFightFlags2 = (unsigned char)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nFightFlags2"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nFightFlags3(lua_State* L) { // m_nFightFlags3 (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nFightFlags3);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nFightFlags3"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nFightFlags3(lua_State* L) { // уст. m_nFightFlags3 (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nFightFlags3 = (unsigned char)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nFightFlags3"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nBleedCounter(lua_State* L) { // m_nBleedCounter (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nBleedCounter);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nBleedCounter"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nBleedCounter(lua_State* L) { // уст. m_nBleedCounter (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nBleedCounter = (unsigned char)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nBleedCounter"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_fLookDirection(lua_State* L) { // m_fLookDirection (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushnumber(L, ped->m_fLookDirection);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_fLookDirection"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_fLookDirection(lua_State* L) { // уст. m_fLookDirection (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_fLookDirection = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_fLookDirection"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nWepModelID(lua_State* L) { // m_nWepModelID (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nWepModelID);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nWepModelID"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nWepModelID(lua_State* L) { // уст. m_nWepModelID (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nWepModelID = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nWepModelID"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nLeaveCarTimer(lua_State* L) { // m_nLeaveCarTimer (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nLeaveCarTimer);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nLeaveCarTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nLeaveCarTimer(lua_State* L) { // уст. m_nLeaveCarTimer (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nLeaveCarTimer = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nLeaveCarTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nGetUpTimer(lua_State* L) { // m_nGetUpTimer (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nGetUpTimer);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nGetUpTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nGetUpTimer(lua_State* L) { // уст. m_nGetUpTimer (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nGetUpTimer = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nGetUpTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nLookTimer(lua_State* L) { // m_nLookTimer (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nLookTimer);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nLookTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nLookTimer(lua_State* L) { // уст. m_nLookTimer (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nLookTimer = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nLookTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nStandardTimer(lua_State* L) { // m_nStandardTimer (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nStandardTimer);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nStandardTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nStandardTimer(lua_State* L) { // уст. m_nStandardTimer (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nStandardTimer = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nStandardTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nAttackTimer(lua_State* L) { // m_nAttackTimer (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nAttackTimer);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nAttackTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nAttackTimer(lua_State* L) { // уст. m_nAttackTimer (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nAttackTimer = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nAttackTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nLastHitTime(lua_State* L) { // m_nLastHitTime (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nLastHitTime);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nLastHitTime"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nLastHitTime(lua_State* L) { // уст. m_nLastHitTime (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nLastHitTime = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nLastHitTime"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nHitRecoverTimer(lua_State* L) { // m_nHitRecoverTimer (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nHitRecoverTimer);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nHitRecoverTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nHitRecoverTimer(lua_State* L) { // уст. m_nHitRecoverTimer (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nHitRecoverTimer = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nHitRecoverTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nObjectiveTimer(lua_State* L) { // m_nObjectiveTimer (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nObjectiveTimer);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nObjectiveTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nObjectiveTimer(lua_State* L) { // уст. m_nObjectiveTimer (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nObjectiveTimer = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nObjectiveTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nDuckTimer(lua_State* L) { // m_nDuckTimer (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nDuckTimer);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nDuckTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nDuckTimer(lua_State* L) { // уст. m_nDuckTimer (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nDuckTimer = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nDuckTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nDuckAndCoverTimer(lua_State* L) { // m_nDuckAndCoverTimer (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nDuckAndCoverTimer);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nDuckAndCoverTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nDuckAndCoverTimer(lua_State* L) { // уст. m_nDuckAndCoverTimer (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nDuckAndCoverTimer = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nDuckAndCoverTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nBloodyTimer(lua_State* L) { // m_nBloodyTimer (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nBloodyTimer);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nBloodyTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nBloodyTimer(lua_State* L) { // уст. m_nBloodyTimer (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nBloodyTimer = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nBloodyTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nShotTime(lua_State* L) { // m_nShotTime (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nShotTime);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nShotTime"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nShotTime(lua_State* L) { // уст. m_nShotTime (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nShotTime = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nShotTime"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nShotTimeAdd(lua_State* L) { // m_nShotTimeAdd (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nShotTimeAdd);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nShotTimeAdd"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nShotTimeAdd(lua_State* L) { // уст. m_nShotTimeAdd (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nShotTimeAdd = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nShotTimeAdd"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nPanicCounter(lua_State* L) { // m_nPanicCounter (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nPanicCounter);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nPanicCounter"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nPanicCounter(lua_State* L) { // уст. m_nPanicCounter (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nPanicCounter = (unsigned char)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nPanicCounter"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nDeadBleeding(lua_State* L) { // m_nDeadBleeding (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nDeadBleeding);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nDeadBleeding"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nDeadBleeding(lua_State* L) { // уст. m_nDeadBleeding (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nDeadBleeding = (unsigned char)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nDeadBleeding"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nBodyPartBleeding(lua_State* L) { // m_nBodyPartBleeding (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nBodyPartBleeding);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nBodyPartBleeding"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nBodyPartBleeding(lua_State* L) { // уст. m_nBodyPartBleeding (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nBodyPartBleeding = (unsigned char)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nBodyPartBleeding"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nNumNearPeds(lua_State* L) { // m_nNumNearPeds (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nNumNearPeds);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nNumNearPeds"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nNumNearPeds(lua_State* L) { // уст. m_nNumNearPeds (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nNumNearPeds = (unsigned short)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nNumNearPeds"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nPedMoney(lua_State* L) { // m_nPedMoney (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nPedMoney);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nPedMoney"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nPedMoney(lua_State* L) { // уст. m_nPedMoney (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nPedMoney = (unsigned short)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nPedMoney"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nLastDamWep(lua_State* L) { // m_nLastDamWep (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nLastDamWep);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nLastDamWep"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nLastDamWep(lua_State* L) { // уст. m_nLastDamWep (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nLastDamWep = (unsigned char)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nLastDamWep"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_vecAttachOffset(lua_State* L) { // m_vecAttachOffset (CPed) (x,y,z)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushnumber(L, ped->m_vecAttachOffset.x);
			lua_pushnumber(L, ped->m_vecAttachOffset.y);
			lua_pushnumber(L, ped->m_vecAttachOffset.z);
			return 3;
		}
		else { throw "bad argument in function get_ped_m_vecAttachOffset"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_vecAttachOffset(lua_State* L) { // уст. m_vecAttachOffset (CPed) (x,y,z)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_vecAttachOffset.x = (float)lua_tonumber(L, 2);
			ped->m_vecAttachOffset.y = (float)lua_tonumber(L, 3);
			ped->m_vecAttachOffset.z = (float)lua_tonumber(L, 4);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_vecAttachOffset"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nAttachType(lua_State* L) { // m_nAttachType (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nAttachType);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nAttachType"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nAttachType(lua_State* L) { // уст. m_nAttachType (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nAttachType = (unsigned short)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nAttachType"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_fAttachRot(lua_State* L) { // m_fAttachRot (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushnumber(L, ped->m_fAttachRot);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_fAttachRot"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_fAttachRot(lua_State* L) { // уст. m_fAttachRot (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_fAttachRot = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_fAttachRot"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nAttachWepAmmo(lua_State* L) { // m_nAttachWepAmmo (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nAttachWepAmmo);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nAttachWepAmmo"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nAttachWepAmmo(lua_State* L) { // уст. m_nAttachWepAmmo (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nAttachWepAmmo = (unsigned int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nAttachWepAmmo"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nThreatFlags(lua_State* L) { // m_nThreatFlags (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nThreatFlags);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nThreatFlags"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nThreatFlags(lua_State* L) { // уст. m_nThreatFlags (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nThreatFlags = (unsigned int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nThreatFlags"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nThreatCheck(lua_State* L) { // m_nThreatCheck (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nThreatCheck);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nThreatCheck"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nThreatCheck(lua_State* L) { // уст. m_nThreatCheck (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nThreatCheck = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nThreatCheck"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nLastThreatCheck(lua_State* L) { // m_nLastThreatCheck (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nLastThreatCheck);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nLastThreatCheck"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nLastThreatCheck(lua_State* L) { // уст. m_nLastThreatCheck (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nLastThreatCheck = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nLastThreatCheck"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nSayType(lua_State* L) { // m_nSayType (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nSayType);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nSayType"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nSayType(lua_State* L) { // уст. m_nSayType (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nSayType = (unsigned int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nSayType"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nSayTimer(lua_State* L) { // m_nSayTimer (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nSayTimer);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nSayTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nSayTimer(lua_State* L) { // уст. m_nSayTimer (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nSayTimer = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nSayTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nTalkTimerLast(lua_State* L) { // m_nTalkTimerLast (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nTalkTimerLast);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nTalkTimerLast"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nTalkTimerLast(lua_State* L) { // уст. m_nTalkTimerLast (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nTalkTimerLast = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nTalkTimerLast"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nTalkTimer(lua_State* L) { // m_nTalkTimer (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nTalkTimer);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nTalkTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nTalkTimer(lua_State* L) { // уст. m_nTalkTimer (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nTalkTimer = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nTalkTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_wTalkTypeLast(lua_State* L) { // m_wTalkTypeLast (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_wTalkTypeLast);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_wTalkTypeLast"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_wTalkTypeLast(lua_State* L) { // уст. m_wTalkTypeLast (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_wTalkTypeLast = (short)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_wTalkTypeLast"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_wTalkType(lua_State* L) { // m_wTalkType (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_wTalkType);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_wTalkType"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_wTalkType(lua_State* L) { // уст. m_wTalkType (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_wTalkType = (unsigned short)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_wTalkType"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_bCanPedTalk(lua_State* L) { // m_bCanPedTalk (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushboolean(L, ped->m_bCanPedTalk);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_bCanPedTalk"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_bCanPedTalk(lua_State* L) { // уст. m_bCanPedTalk (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_bCanPedTalk = (bool)lua_toboolean(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_bCanPedTalk"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_nPedLastComment(lua_State* L) { // m_nPedLastComment (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)ped->m_nPedLastComment);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_nPedLastComment"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_nPedLastComment(lua_State* L) { // уст. m_nPedLastComment (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_nPedLastComment = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_nPedLastComment"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_vecSeekPosEx(lua_State* L) { // m_vecSeekPosEx (CPed) (x,y,z)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushnumber(L, ped->m_vecSeekPosEx.x);
			lua_pushnumber(L, ped->m_vecSeekPosEx.y);
			lua_pushnumber(L, ped->m_vecSeekPosEx.z);
			return 3;
		}
		else { throw "bad argument in function get_ped_m_vecSeekPosEx"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_vecSeekPosEx(lua_State* L) { // уст. m_vecSeekPosEx (CPed) (x,y,z)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_vecSeekPosEx.x = (float)lua_tonumber(L, 2);
			ped->m_vecSeekPosEx.y = (float)lua_tonumber(L, 3);
			ped->m_vecSeekPosEx.z = (float)lua_tonumber(L, 4);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_vecSeekPosEx"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_ped_m_fSeekExAngle(lua_State* L) { // m_fSeekExAngle (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			lua_pushnumber(L, ped->m_fSeekExAngle);
			return 1;
		}
		else { throw "bad argument in function get_ped_m_fSeekExAngle"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_ped_m_fSeekExAngle(lua_State* L) { // уст. m_fSeekExAngle (CPed)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = findpedinpool(lua_topointer(L, 1));
			ped->m_fSeekExAngle = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_ped_m_fSeekExAngle"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_obj_m_nObjectType(lua_State* L) { // m_nObjectType (CObject)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CObject* obj = findobjinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)obj->m_nObjectType);
			return 1;
		}
		else { throw "bad argument in function get_obj_m_nObjectType"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_obj_m_nObjectType(lua_State* L) { // уст. m_nObjectType (CObject)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CObject* obj = findobjinpool(lua_topointer(L, 1));
			obj->m_nObjectType = (unsigned char)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_obj_m_nObjectType"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_obj_m_nBonusValue(lua_State* L) { // m_nBonusValue (CObject)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CObject* obj = findobjinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)obj->m_nBonusValue);
			return 1;
		}
		else { throw "bad argument in function get_obj_m_nBonusValue"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_obj_m_nBonusValue(lua_State* L) { // уст. m_nBonusValue (CObject)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CObject* obj = findobjinpool(lua_topointer(L, 1));
			obj->m_nBonusValue = (unsigned char)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_obj_m_nBonusValue"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_obj_m_wCostValue(lua_State* L) { // m_wCostValue (CObject)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CObject* obj = findobjinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)obj->m_wCostValue);
			return 1;
		}
		else { throw "bad argument in function get_obj_m_wCostValue"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_obj_m_wCostValue(lua_State* L) { // уст. m_wCostValue (CObject)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CObject* obj = findobjinpool(lua_topointer(L, 1));
			obj->m_wCostValue = (unsigned short)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_obj_m_wCostValue"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_obj_m_fDamageMultiplier(lua_State* L) { // m_fDamageMultiplier (CObject)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CObject* obj = findobjinpool(lua_topointer(L, 1));
			lua_pushnumber(L, obj->m_fDamageMultiplier);
			return 1;
		}
		else { throw "bad argument in function get_obj_m_fDamageMultiplier"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_obj_m_fDamageMultiplier(lua_State* L) { // уст. m_fDamageMultiplier (CObject)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CObject* obj = findobjinpool(lua_topointer(L, 1));
			obj->m_fDamageMultiplier = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_obj_m_fDamageMultiplier"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_obj_m_bCameraAvoids(lua_State* L) { // m_bCameraAvoids (CObject)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CObject* obj = findobjinpool(lua_topointer(L, 1));
			lua_pushboolean(L, obj->m_bCameraAvoids);
			return 1;
		}
		else { throw "bad argument in function get_obj_m_bCameraAvoids"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_obj_m_bCameraAvoids(lua_State* L) { // уст. m_bCameraAvoids (CObject)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {
			CObject* obj = findobjinpool(lua_topointer(L, 1));
			obj->m_bCameraAvoids = (bool)lua_toboolean(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_obj_m_bCameraAvoids"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_obj_m_wRefModelId(lua_State* L) { // m_wRefModelId (CObject)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CObject* obj = findobjinpool(lua_topointer(L, 1));
			lua_pushinteger(L, (lua_Integer)obj->m_wRefModelId);
			return 1;
		}
		else { throw "bad argument in function get_obj_m_wRefModelId"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_obj_m_wRefModelId(lua_State* L) { // уст. m_wRefModelId (CObject)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CObject* obj = findobjinpool(lua_topointer(L, 1));
			obj->m_wRefModelId = (short)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_obj_m_wRefModelId"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_nAudioEntityId(lua_State* L) { // m_nAudioEntityId (CPhysical)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			lua_pushinteger(L, (lua_Integer)((CPhysical*)car)->m_nAudioEntityId);
			return 1;
		}
		else { throw "bad argument in function get_car_m_nAudioEntityId"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_nAudioEntityId(lua_State* L) { // уст. m_nAudioEntityId (CPhysical)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			((CPhysical*)car)->m_nAudioEntityId = (unsigned int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_nAudioEntityId"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_vecMoveSpeed(lua_State* L) { // m_vecMoveSpeed (CPhysical) (x,y,z)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			lua_pushnumber(L, ((CPhysical*)car)->m_vecMoveSpeed.x);
			lua_pushnumber(L, ((CPhysical*)car)->m_vecMoveSpeed.y);
			lua_pushnumber(L, ((CPhysical*)car)->m_vecMoveSpeed.z);
			return 3;
		}
		else { throw "bad argument in function get_car_m_vecMoveSpeed"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_vecMoveSpeed(lua_State* L) { // уст. m_vecMoveSpeed (CPhysical) (x,y,z)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			((CPhysical*)car)->m_vecMoveSpeed.x = (float)lua_tonumber(L, 2);
			((CPhysical*)car)->m_vecMoveSpeed.y = (float)lua_tonumber(L, 3);
			((CPhysical*)car)->m_vecMoveSpeed.z = (float)lua_tonumber(L, 4);
			return 0;
		}
		else { throw "bad argument in function set_car_m_vecMoveSpeed"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_vecTurnSpeed(lua_State* L) { // m_vecTurnSpeed (CPhysical) (x,y,z)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			lua_pushnumber(L, ((CPhysical*)car)->m_vecTurnSpeed.x);
			lua_pushnumber(L, ((CPhysical*)car)->m_vecTurnSpeed.y);
			lua_pushnumber(L, ((CPhysical*)car)->m_vecTurnSpeed.z);
			return 3;
		}
		else { throw "bad argument in function get_car_m_vecTurnSpeed"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_vecTurnSpeed(lua_State* L) { // уст. m_vecTurnSpeed (CPhysical) (x,y,z)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			((CPhysical*)car)->m_vecTurnSpeed.x = (float)lua_tonumber(L, 2);
			((CPhysical*)car)->m_vecTurnSpeed.y = (float)lua_tonumber(L, 3);
			((CPhysical*)car)->m_vecTurnSpeed.z = (float)lua_tonumber(L, 4);
			return 0;
		}
		else { throw "bad argument in function set_car_m_vecTurnSpeed"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_vecFrictionMoveForce(lua_State* L) { // m_vecFrictionMoveForce (CPhysical) (x,y,z)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			lua_pushnumber(L, ((CPhysical*)car)->m_vecFrictionMoveForce.x);
			lua_pushnumber(L, ((CPhysical*)car)->m_vecFrictionMoveForce.y);
			lua_pushnumber(L, ((CPhysical*)car)->m_vecFrictionMoveForce.z);
			return 3;
		}
		else { throw "bad argument in function get_car_m_vecFrictionMoveForce"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_vecFrictionMoveForce(lua_State* L) { // уст. m_vecFrictionMoveForce (CPhysical) (x,y,z)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			((CPhysical*)car)->m_vecFrictionMoveForce.x = (float)lua_tonumber(L, 2);
			((CPhysical*)car)->m_vecFrictionMoveForce.y = (float)lua_tonumber(L, 3);
			((CPhysical*)car)->m_vecFrictionMoveForce.z = (float)lua_tonumber(L, 4);
			return 0;
		}
		else { throw "bad argument in function set_car_m_vecFrictionMoveForce"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_vecFrictionTurnForce(lua_State* L) { // m_vecFrictionTurnForce (CPhysical) (x,y,z)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			lua_pushnumber(L, ((CPhysical*)car)->m_vecFrictionTurnForce.x);
			lua_pushnumber(L, ((CPhysical*)car)->m_vecFrictionTurnForce.y);
			lua_pushnumber(L, ((CPhysical*)car)->m_vecFrictionTurnForce.z);
			return 3;
		}
		else { throw "bad argument in function get_car_m_vecFrictionTurnForce"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_vecFrictionTurnForce(lua_State* L) { // уст. m_vecFrictionTurnForce (CPhysical) (x,y,z)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			((CPhysical*)car)->m_vecFrictionTurnForce.x = (float)lua_tonumber(L, 2);
			((CPhysical*)car)->m_vecFrictionTurnForce.y = (float)lua_tonumber(L, 3);
			((CPhysical*)car)->m_vecFrictionTurnForce.z = (float)lua_tonumber(L, 4);
			return 0;
		}
		else { throw "bad argument in function set_car_m_vecFrictionTurnForce"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_vecForce(lua_State* L) { // m_vecForce (CPhysical) (x,y,z)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			lua_pushnumber(L, ((CPhysical*)car)->m_vecForce.x);
			lua_pushnumber(L, ((CPhysical*)car)->m_vecForce.y);
			lua_pushnumber(L, ((CPhysical*)car)->m_vecForce.z);
			return 3;
		}
		else { throw "bad argument in function get_car_m_vecForce"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_vecForce(lua_State* L) { // уст. m_vecForce (CPhysical) (x,y,z)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			((CPhysical*)car)->m_vecForce.x = (float)lua_tonumber(L, 2);
			((CPhysical*)car)->m_vecForce.y = (float)lua_tonumber(L, 3);
			((CPhysical*)car)->m_vecForce.z = (float)lua_tonumber(L, 4);
			return 0;
		}
		else { throw "bad argument in function set_car_m_vecForce"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_vecTorque(lua_State* L) { // m_vecTorque (CPhysical) (x,y,z)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			lua_pushnumber(L, ((CPhysical*)car)->m_vecTorque.x);
			lua_pushnumber(L, ((CPhysical*)car)->m_vecTorque.y);
			lua_pushnumber(L, ((CPhysical*)car)->m_vecTorque.z);
			return 3;
		}
		else { throw "bad argument in function get_car_m_vecTorque"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_vecTorque(lua_State* L) { // уст. m_vecTorque (CPhysical) (x,y,z)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			((CPhysical*)car)->m_vecTorque.x = (float)lua_tonumber(L, 2);
			((CPhysical*)car)->m_vecTorque.y = (float)lua_tonumber(L, 3);
			((CPhysical*)car)->m_vecTorque.z = (float)lua_tonumber(L, 4);
			return 0;
		}
		else { throw "bad argument in function set_car_m_vecTorque"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_fMass(lua_State* L) { // m_fMass (CPhysical)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			lua_pushnumber(L, ((CPhysical*)car)->m_fMass);
			return 1;
		}
		else { throw "bad argument in function get_car_m_fMass"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_fMass(lua_State* L) { // уст. m_fMass (CPhysical)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			((CPhysical*)car)->m_fMass = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_fMass"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_fTurnMass(lua_State* L) { // m_fTurnMass (CPhysical)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			lua_pushnumber(L, ((CPhysical*)car)->m_fTurnMass);
			return 1;
		}
		else { throw "bad argument in function get_car_m_fTurnMass"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_fTurnMass(lua_State* L) { // уст. m_fTurnMass (CPhysical)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			((CPhysical*)car)->m_fTurnMass = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_fTurnMass"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_fVelocityFrequency(lua_State* L) { // m_fVelocityFrequency (CPhysical)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			lua_pushnumber(L, ((CPhysical*)car)->m_fVelocityFrequency);
			return 1;
		}
		else { throw "bad argument in function get_car_m_fVelocityFrequency"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_fVelocityFrequency(lua_State* L) { // уст. m_fVelocityFrequency (CPhysical)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			((CPhysical*)car)->m_fVelocityFrequency = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_fVelocityFrequency"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_fAirResistance(lua_State* L) { // m_fAirResistance (CPhysical)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			lua_pushnumber(L, ((CPhysical*)car)->m_fAirResistance);
			return 1;
		}
		else { throw "bad argument in function get_car_m_fAirResistance"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_fAirResistance(lua_State* L) { // уст. m_fAirResistance (CPhysical)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			((CPhysical*)car)->m_fAirResistance = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_fAirResistance"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_fElasticity(lua_State* L) { // m_fElasticity (CPhysical)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			lua_pushnumber(L, ((CPhysical*)car)->m_fElasticity);
			return 1;
		}
		else { throw "bad argument in function get_car_m_fElasticity"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_fElasticity(lua_State* L) { // уст. m_fElasticity (CPhysical)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			((CPhysical*)car)->m_fElasticity = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_fElasticity"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_fBuoyancyConstant(lua_State* L) { // m_fBuoyancyConstant (CPhysical)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			lua_pushnumber(L, ((CPhysical*)car)->m_fBuoyancyConstant);
			return 1;
		}
		else { throw "bad argument in function get_car_m_fBuoyancyConstant"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_fBuoyancyConstant(lua_State* L) { // уст. m_fBuoyancyConstant (CPhysical)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			((CPhysical*)car)->m_fBuoyancyConstant = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_fBuoyancyConstant"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_vecCentreOfMass(lua_State* L) { // m_vecCentreOfMass (CPhysical) (x,y,z)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			lua_pushnumber(L, ((CPhysical*)car)->m_vecCentreOfMass.x);
			lua_pushnumber(L, ((CPhysical*)car)->m_vecCentreOfMass.y);
			lua_pushnumber(L, ((CPhysical*)car)->m_vecCentreOfMass.z);
			return 3;
		}
		else { throw "bad argument in function get_car_m_vecCentreOfMass"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_vecCentreOfMass(lua_State* L) { // уст. m_vecCentreOfMass (CPhysical) (x,y,z)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			((CPhysical*)car)->m_vecCentreOfMass.x = (float)lua_tonumber(L, 2);
			((CPhysical*)car)->m_vecCentreOfMass.y = (float)lua_tonumber(L, 3);
			((CPhysical*)car)->m_vecCentreOfMass.z = (float)lua_tonumber(L, 4);
			return 0;
		}
		else { throw "bad argument in function set_car_m_vecCentreOfMass"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_nNumCollisionRecords(lua_State* L) { // m_nNumCollisionRecords (CPhysical)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			lua_pushinteger(L, (lua_Integer)((CPhysical*)car)->m_nNumCollisionRecords);
			return 1;
		}
		else { throw "bad argument in function get_car_m_nNumCollisionRecords"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_nNumCollisionRecords(lua_State* L) { // уст. m_nNumCollisionRecords (CPhysical)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			((CPhysical*)car)->m_nNumCollisionRecords = (unsigned char)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_nNumCollisionRecords"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_fTotSpeed(lua_State* L) { // m_fTotSpeed (CPhysical)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			lua_pushnumber(L, ((CPhysical*)car)->m_fTotSpeed);
			return 1;
		}
		else { throw "bad argument in function get_car_m_fTotSpeed"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_fTotSpeed(lua_State* L) { // уст. m_fTotSpeed (CPhysical)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			((CPhysical*)car)->m_fTotSpeed = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_fTotSpeed"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_fCollisionPower(lua_State* L) { // m_fCollisionPower (CPhysical)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			lua_pushnumber(L, ((CPhysical*)car)->m_fCollisionPower);
			return 1;
		}
		else { throw "bad argument in function get_car_m_fCollisionPower"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_fCollisionPower(lua_State* L) { // уст. m_fCollisionPower (CPhysical)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			((CPhysical*)car)->m_fCollisionPower = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_fCollisionPower"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_vecCollisionPower(lua_State* L) { // m_vecCollisionPower (CPhysical) (x,y,z)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			lua_pushnumber(L, ((CPhysical*)car)->m_vecCollisionPower.x);
			lua_pushnumber(L, ((CPhysical*)car)->m_vecCollisionPower.y);
			lua_pushnumber(L, ((CPhysical*)car)->m_vecCollisionPower.z);
			return 3;
		}
		else { throw "bad argument in function get_car_m_vecCollisionPower"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_vecCollisionPower(lua_State* L) { // уст. m_vecCollisionPower (CPhysical) (x,y,z)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			((CPhysical*)car)->m_vecCollisionPower.x = (float)lua_tonumber(L, 2);
			((CPhysical*)car)->m_vecCollisionPower.y = (float)lua_tonumber(L, 3);
			((CPhysical*)car)->m_vecCollisionPower.z = (float)lua_tonumber(L, 4);
			return 0;
		}
		else { throw "bad argument in function set_car_m_vecCollisionPower"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_wComponentCol(lua_State* L) { // m_wComponentCol (CPhysical)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			lua_pushinteger(L, (lua_Integer)((CPhysical*)car)->m_wComponentCol);
			return 1;
		}
		else { throw "bad argument in function get_car_m_wComponentCol"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_wComponentCol(lua_State* L) { // уст. m_wComponentCol (CPhysical)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			((CPhysical*)car)->m_wComponentCol = (unsigned short)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_wComponentCol"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_nMoveFlags(lua_State* L) { // m_nMoveFlags (CPhysical)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			lua_pushinteger(L, (lua_Integer)((CPhysical*)car)->m_nMoveFlags);
			return 1;
		}
		else { throw "bad argument in function get_car_m_nMoveFlags"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_nMoveFlags(lua_State* L) { // уст. m_nMoveFlags (CPhysical)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			((CPhysical*)car)->m_nMoveFlags = (unsigned char)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_nMoveFlags"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_nCollFlags(lua_State* L) { // m_nCollFlags (CPhysical)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			lua_pushinteger(L, (lua_Integer)((CPhysical*)car)->m_nCollFlags);
			return 1;
		}
		else { throw "bad argument in function get_car_m_nCollFlags"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_nCollFlags(lua_State* L) { // уст. m_nCollFlags (CPhysical)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			((CPhysical*)car)->m_nCollFlags = (unsigned char)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_nCollFlags"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_nLastCollType(lua_State* L) { // m_nLastCollType (CPhysical)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			lua_pushinteger(L, (lua_Integer)((CPhysical*)car)->m_nLastCollType);
			return 1;
		}
		else { throw "bad argument in function get_car_m_nLastCollType"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_nLastCollType(lua_State* L) { // уст. m_nLastCollType (CPhysical)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			((CPhysical*)car)->m_nLastCollType = (unsigned char)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_nLastCollType"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_nZoneLevel(lua_State* L) { // m_nZoneLevel (CPhysical)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			lua_pushinteger(L, (lua_Integer)((CPhysical*)car)->m_nZoneLevel);
			return 1;
		}
		else { throw "bad argument in function get_car_m_nZoneLevel"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_nZoneLevel(lua_State* L) { // уст. m_nZoneLevel (CPhysical)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			(CPhysical*)car;
			((CPhysical*)car)->m_nZoneLevel = (unsigned char)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_nZoneLevel"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_fNegSpeed(lua_State* L) { // fNegSpeed (CAutomobile)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_AUTOMOBILE) throw "not an automobile";
			(CAutomobile*)car;
			lua_pushinteger(L, (lua_Integer)((CAutomobile*)car)->fNegSpeed);
			return 1;
		}
		else { throw "bad argument in function get_car_fNegSpeed"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_fNegSpeed(lua_State* L) { // уст. fNegSpeed (CAutomobile)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_AUTOMOBILE) throw "not an automobile";
			(CAutomobile*)car;
			((CAutomobile*)car)->fNegSpeed = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_fNegSpeed"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_fAIGripMultiplier(lua_State* L) { // fAIGripMultiplier (CAutomobile)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_AUTOMOBILE) throw "not an automobile";
			(CAutomobile*)car;
			lua_pushinteger(L, (lua_Integer)((CAutomobile*)car)->fAIGripMultiplier);
			return 1;
		}
		else { throw "bad argument in function get_car_fAIGripMultiplier"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_fAIGripMultiplier(lua_State* L) { // уст. fAIGripMultiplier (CAutomobile)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_AUTOMOBILE) throw "not an automobile";
			(CAutomobile*)car;
			((CAutomobile*)car)->fAIGripMultiplier = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_fAIGripMultiplier"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_fSpecialWepRotH(lua_State* L) { // fSpecialWepRotH (CAutomobile)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_AUTOMOBILE) throw "not an automobile";
			(CAutomobile*)car;
			lua_pushinteger(L, (lua_Integer)((CAutomobile*)car)->fSpecialWepRotH);
			return 1;
		}
		else { throw "bad argument in function get_car_fSpecialWepRotH"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_fSpecialWepRotH(lua_State* L) { // уст. fSpecialWepRotH (CAutomobile)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_AUTOMOBILE) throw "not an automobile";
			(CAutomobile*)car;
			((CAutomobile*)car)->fSpecialWepRotH = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_fSpecialWepRotH"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_fSpecialWepRotV(lua_State* L) { // fSpecialWepRotV (CAutomobile)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_AUTOMOBILE) throw "not an automobile";
			(CAutomobile*)car;
			lua_pushinteger(L, (lua_Integer)((CAutomobile*)car)->fSpecialWepRotV);
			return 1;
		}
		else { throw "bad argument in function get_car_fSpecialWepRotV"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_fSpecialWepRotV(lua_State* L) { // уст. fSpecialWepRotV (CAutomobile)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_AUTOMOBILE) throw "not an automobile";
			(CAutomobile*)car;
			((CAutomobile*)car)->fSpecialWepRotV = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_fSpecialWepRotV"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_fSpecialSteering(lua_State* L) { // fSpecialSteering (CAutomobile)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_AUTOMOBILE) throw "not an automobile";
			(CAutomobile*)car;
			lua_pushinteger(L, (lua_Integer)((CAutomobile*)car)->fSpecialSteering);
			return 1;
		}
		else { throw "bad argument in function get_car_fSpecialSteering"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_fSpecialSteering(lua_State* L) { // уст. fSpecialSteering (CAutomobile)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_AUTOMOBILE) throw "not an automobile";
			(CAutomobile*)car;
			((CAutomobile*)car)->fSpecialSteering = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_fSpecialSteering"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_fSpecialMoveState(lua_State* L) { // fSpecialMoveState (CAutomobile)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_AUTOMOBILE) throw "not an automobile";
			(CAutomobile*)car;
			lua_pushinteger(L, (lua_Integer)((CAutomobile*)car)->fSpecialMoveState);
			return 1;
		}
		else { throw "bad argument in function get_car_fSpecialMoveState"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_fSpecialMoveState(lua_State* L) { // уст. fSpecialMoveState (CAutomobile)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_AUTOMOBILE) throw "not an automobile";
			(CAutomobile*)car;
			((CAutomobile*)car)->fSpecialMoveState = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_fSpecialMoveState"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_fSkidMarkDensity(lua_State* L) { // fSkidMarkDensity (CAutomobile)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_AUTOMOBILE) throw "not an automobile";
			(CAutomobile*)car;
			lua_pushinteger(L, (lua_Integer)((CAutomobile*)car)->fSkidMarkDensity);
			return 1;
		}
		else { throw "bad argument in function get_car_fSkidMarkDensity"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_fSkidMarkDensity(lua_State* L) { // уст. fSkidMarkDensity (CAutomobile)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_AUTOMOBILE) throw "not an automobile";
			(CAutomobile*)car;
			((CAutomobile*)car)->fSkidMarkDensity = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_fSkidMarkDensity"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_fPropRotation(lua_State* L) { // m_fPropRotation (CBoat)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_BOAT) throw "not a boat";
			(CBoat*)car;
			lua_pushnumber(L, ((CBoat*)car)->m_fPropRotation);
			return 1;
		}
		else { throw "bad argument in function get_car_m_fPropRotation"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_fPropRotation(lua_State* L) { // уст. m_fPropRotation (CBoat)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_BOAT) throw "not a boat";
			(CBoat*)car;
			((CBoat*)car)->m_fPropRotation = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_fPropRotation"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_fPropSpeed(lua_State* L) { // m_fPropSpeed (CBoat)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_BOAT) throw "not a boat";
			(CBoat*)car;
			lua_pushnumber(L, ((CBoat*)car)->m_fPropSpeed);
			return 1;
		}
		else { throw "bad argument in function get_car_m_fPropSpeed"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_fPropSpeed(lua_State* L) { // уст. m_fPropSpeed (CBoat)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_BOAT) throw "not a boat";
			(CBoat*)car;
			((CBoat*)car)->m_fPropSpeed = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_fPropSpeed"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_fForcedZRotation(lua_State* L) { // m_fForcedZRotation (CBoat)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_BOAT) throw "not a boat";
			(CBoat*)car;
			lua_pushnumber(L, ((CBoat*)car)->m_fForcedZRotation);
			return 1;
		}
		else { throw "bad argument in function get_car_m_fForcedZRotation"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_fForcedZRotation(lua_State* L) { // уст. m_fForcedZRotation (CBoat)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_BOAT) throw "not a boat";
			(CBoat*)car;
			((CBoat*)car)->m_fForcedZRotation = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_fForcedZRotation"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_nAttackPlayerTime(lua_State* L) { // m_nAttackPlayerTime (CBoat)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_BOAT) throw "not a boat";
			(CBoat*)car;
			lua_pushinteger(L, (lua_Integer)((CBoat*)car)->m_nAttackPlayerTime);
			return 1;
		}
		else { throw "bad argument in function get_car_m_nAttackPlayerTime"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_nAttackPlayerTime(lua_State* L) { // уст. m_nAttackPlayerTime (CBoat)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_BOAT) throw "not a boat";
			(CBoat*)car;
			((CBoat*)car)->m_nAttackPlayerTime = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_nAttackPlayerTime"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_fBurningTimer(lua_State* L) { // m_fBurningTimer (CBoat)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_BOAT) throw "not a boat";
			(CBoat*)car;
			lua_pushnumber(L, ((CBoat*)car)->m_fBurningTimer);
			return 1;
		}
		else { throw "bad argument in function get_car_m_fBurningTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_fBurningTimer(lua_State* L) { // уст. m_fBurningTimer (CBoat)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_BOAT) throw "not a boat";
			(CBoat*)car;
			((CBoat*)car)->m_fBurningTimer = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_fBurningTimer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_fBoatGasPedal(lua_State* L) { // m_fBoatGasPedal (CBoat)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_BOAT) throw "not a boat";
			(CBoat*)car;
			lua_pushnumber(L, ((CBoat*)car)->m_fBoatGasPedal);
			return 1;
		}
		else { throw "bad argument in function get_car_m_fBoatGasPedal"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_fBoatGasPedal(lua_State* L) { // уст. m_fBoatGasPedal (CBoat)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_BOAT) throw "not a boat";
			(CBoat*)car;
			((CBoat*)car)->m_fBoatGasPedal = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_fBoatGasPedal"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_fBoatBrakePedal(lua_State* L) { // m_fBoatBrakePedal (CBoat)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_BOAT) throw "not a boat";
			(CBoat*)car;
			lua_pushnumber(L, ((CBoat*)car)->m_fBoatBrakePedal);
			return 1;
		}
		else { throw "bad argument in function get_car_m_fBoatBrakePedal"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_fBoatBrakePedal(lua_State* L) { // уст. m_fBoatBrakePedal (CBoat)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_BOAT) throw "not a boat";
			(CBoat*)car;
			((CBoat*)car)->m_fBoatBrakePedal = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_fBoatBrakePedal"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_fBoatSteeringLeftRight(lua_State* L) { // m_fBoatSteeringLeftRight (CBoat)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_BOAT) throw "not a boat";
			(CBoat*)car;
			lua_pushnumber(L, ((CBoat*)car)->m_fBoatSteeringLeftRight);
			return 1;
		}
		else { throw "bad argument in function get_car_m_fBoatSteeringLeftRight"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_fBoatSteeringLeftRight(lua_State* L) { // уст. m_fBoatSteeringLeftRight (CBoat)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_BOAT) throw "not a boat";
			(CBoat*)car;
			((CBoat*)car)->m_fBoatSteeringLeftRight = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_fBoatSteeringLeftRight"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_nPadNumber(lua_State* L) { // m_nPadNumber (CBoat)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_BOAT) throw "not a boat";
			(CBoat*)car;
			lua_pushinteger(L, (lua_Integer)((CBoat*)car)->m_nPadNumber);
			return 1;
		}
		else { throw "bad argument in function get_car_m_nPadNumber"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_nPadNumber(lua_State* L) { // уст. m_nPadNumber (CBoat)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_BOAT) throw "not a boat";
			(CBoat*)car;
			((CBoat*)car)->m_nPadNumber = (unsigned char)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_nPadNumber"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_nNumWaterTrailPoints(lua_State* L) { // m_nNumWaterTrailPoints (CBoat)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_BOAT) throw "not a boat";
			(CBoat*)car;
			lua_pushinteger(L, (lua_Integer)((CBoat*)car)->m_nNumWaterTrailPoints);
			return 1;
		}
		else { throw "bad argument in function get_car_m_nNumWaterTrailPoints"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_nNumWaterTrailPoints(lua_State* L) { // уст. m_nNumWaterTrailPoints (CBoat)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_BOAT) throw "not a boat";
			(CBoat*)car;
			((CBoat*)car)->m_nNumWaterTrailPoints = (unsigned short)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_nNumWaterTrailPoints"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_nBikeAnimGroup(lua_State* L) { // nBikeAnimGroup (CBike)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_BIKE) throw "not a bike";
			(CBike*)car;
			lua_pushinteger(L, (lua_Integer)((CBike*)car)->nBikeAnimGroup);
			return 1;
		}
		else { throw "bad argument in function get_car_nBikeAnimGroup"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_nBikeAnimGroup(lua_State* L) { // уст. nBikeAnimGroup (CBike)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_BIKE) throw "not a bike";
			(CBike*)car;
			((CBike*)car)->nBikeAnimGroup = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_nBikeAnimGroup"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_nDamageFlags(lua_State* L) { // m_nDamageFlags (CBike)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_BIKE) throw "not a bike";
			(CBike*)car;
			lua_pushinteger(L, (lua_Integer)((CBike*)car)->m_nDamageFlags);
			return 1;
		}
		else { throw "bad argument in function get_car_m_nDamageFlags"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_nDamageFlags(lua_State* L) { // уст. m_nDamageFlags (CBike)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_BIKE) throw "not a bike";
			(CBike*)car;
			((CBike*)car)->m_nDamageFlags = (unsigned char)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_nDamageFlags"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_fSearchLightX(lua_State* L) { // m_fSearchLightX (CHeli)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_HELI) throw "not a heli";
			(CHeli*)car;
			lua_pushnumber(L, ((CHeli*)car)->m_fSearchLightX);
			return 1;
		}
		else { throw "bad argument in function get_car_m_fSearchLightX"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_fSearchLightX(lua_State* L) { // уст. m_fSearchLightX (CHeli)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_HELI) throw "not a heli";
			(CHeli*)car;
			((CHeli*)car)->m_fSearchLightX = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_fSearchLightX"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_fSearchLightY(lua_State* L) { // m_fSearchLightY (CHeli)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_HELI) throw "not a heli";
			(CHeli*)car;
			lua_pushnumber(L, ((CHeli*)car)->m_fSearchLightY);
			return 1;
		}
		else { throw "bad argument in function get_car_m_fSearchLightY"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_fSearchLightY(lua_State* L) { // уст. m_fSearchLightY (CHeli)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_HELI) throw "not a heli";
			(CHeli*)car;
			((CHeli*)car)->m_fSearchLightY = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_fSearchLightY"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_nExplosionTime(lua_State* L) { // m_nExplosionTime (CHeli)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_HELI) throw "not a heli";
			(CHeli*)car;
			lua_pushinteger(L, (lua_Integer)((CHeli*)car)->m_nExplosionTime);
			return 1;
		}
		else { throw "bad argument in function get_car_m_nExplosionTime"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_nExplosionTime(lua_State* L) { // уст. m_nExplosionTime (CHeli)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_HELI) throw "not a heli";
			(CHeli*)car;
			((CHeli*)car)->m_nExplosionTime = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_nExplosionTime"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_fRotationX(lua_State* L) { // m_fRotationX (CHeli)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_HELI) throw "not a heli";
			(CHeli*)car;
			lua_pushnumber(L, ((CHeli*)car)->m_fRotationX);
			return 1;
		}
		else { throw "bad argument in function get_car_m_fRotationX"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_fRotationX(lua_State* L) { // уст. m_fRotationX (CHeli)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_HELI) throw "not a heli";
			(CHeli*)car;
			((CHeli*)car)->m_fRotationX = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_fRotationX"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_fHeliForcedZ(lua_State* L) { // m_fHeliForcedZ (CHeli)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_HELI) throw "not a heli";
			(CHeli*)car;
			lua_pushnumber(L, ((CHeli*)car)->m_fHeliForcedZ);
			return 1;
		}
		else { throw "bad argument in function get_car_m_fHeliForcedZ"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_fHeliForcedZ(lua_State* L) { // уст. m_fHeliForcedZ (CHeli)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_HELI) throw "not a heli";
			(CHeli*)car;
			((CHeli*)car)->m_fHeliForcedZ = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_fHeliForcedZ"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_fSearchLightIntensity(lua_State* L) { // m_fSearchLightIntensity (CHeli)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_HELI) throw "not a heli";
			(CHeli*)car;
			lua_pushnumber(L, ((CHeli*)car)->m_fSearchLightIntensity);
			return 1;
		}
		else { throw "bad argument in function get_car_m_fSearchLightIntensity"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_fSearchLightIntensity(lua_State* L) { // уст. m_fSearchLightIntensity (CHeli)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_HELI) throw "not a heli";
			(CHeli*)car;
			((CHeli*)car)->m_fSearchLightIntensity = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_fSearchLightIntensity"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_nDamagePoints(lua_State* L) { // m_nDamagePoints (CHeli)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_HELI) throw "not a heli";
			(CHeli*)car;
			lua_pushinteger(L, (lua_Integer)((CHeli*)car)->m_nDamagePoints);
			return 1;
		}
		else { throw "bad argument in function get_car_m_nDamagePoints"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_nDamagePoints(lua_State* L) { // уст. m_nDamagePoints (CHeli)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_HELI) throw "not a heli";
			(CHeli*)car;
			((CHeli*)car)->m_nDamagePoints = (int)lua_tointeger(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_nDamagePoints"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_car_m_fRotorAngle(lua_State* L) { // m_fRotorAngle (CHeli)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_HELI) throw "not a heli";
			(CHeli*)car;
			lua_pushnumber(L, ((CHeli*)car)->m_fRotorAngle);
			return 1;
		}
		else { throw "bad argument in function get_car_m_fRotorAngle"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_car_m_fRotorAngle(lua_State* L) { // уст. m_fRotorAngle (CHeli)
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = findcarinpool(lua_topointer(L, 1));
			if (car->m_nVehicleClass != VEHICLE_HELI) throw "not a heli";
			(CHeli*)car;
			((CHeli*)car)->m_fRotorAngle = (float)lua_tonumber(L, 2);
			return 0;
		}
		else { throw "bad argument in function set_car_m_fRotorAngle"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_wanted_m_nLastTimeWantedDecreased(lua_State* L) { // последнее снижение розыска
	try {
		if (true) {
			lua_pushinteger(L, (lua_Integer)FindPlayerPed()->GetWanted()->m_nLastTimeWantedDecreased);
			return 1;
		}
		else { throw "bad argument in function get_wanted_m_nLastTimeWantedDecreased"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_wanted_m_nLastTimeWantedLevelChanged(lua_State* L) { // последняя смена уровня розыска
	try {
		if (true) {
			lua_pushinteger(L, (lua_Integer)FindPlayerPed()->GetWanted()->m_nLastTimeWantedLevelChanged);
			return 1;
		}
		else { throw "bad argument in function get_wanted_m_nLastTimeWantedLevelChanged"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_wanted_m_dwTimeOfParole(lua_State* L) { // таймер условного освобождения
	try {
		if (true) {
			lua_pushinteger(L, (lua_Integer)FindPlayerPed()->GetWanted()->m_dwTimeOfParole);
			return 1;
		}
		else { throw "bad argument in function get_wanted_m_dwTimeOfParole"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_wanted_m_dwTimeOfParole(lua_State* L) { // уст. таймер условного освобождения
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {
			FindPlayerPed()->GetWanted()->m_dwTimeOfParole = (int)lua_tointeger(L, 1);
		}
		else { throw "bad argument in function set_wanted_m_dwTimeOfParole"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_wanted_m_fMultiplier(lua_State* L) { // множитель розыска
	try {
		if (true) {
			lua_pushnumber(L, FindPlayerPed()->GetWanted()->m_fMultiplier);
			return 1;
		}
		else { throw "bad argument in function get_wanted_m_fMultiplier"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_wanted_m_fMultiplier(lua_State* L) { // уст. множитель розыска
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {
			FindPlayerPed()->GetWanted()->m_fMultiplier = (float)lua_tonumber(L, 1);
		}
		else { throw "bad argument in function set_wanted_m_fMultiplier"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_wanted_m_nCopsInPursuit(lua_State* L) { // копов в погоне
	try {
		if (true) {
			lua_pushinteger(L, (lua_Integer)FindPlayerPed()->GetWanted()->m_nCopsInPursuit);
			return 1;
		}
		else { throw "bad argument in function get_wanted_m_nCopsInPursuit"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_wanted_m_nMaxCopsInPursuit(lua_State* L) { // макс. копов в погоне
	try {
		if (true) {
			lua_pushinteger(L, (lua_Integer)FindPlayerPed()->GetWanted()->m_nMaxCopsInPursuit);
			return 1;
		}
		else { throw "bad argument in function get_wanted_m_nMaxCopsInPursuit"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_wanted_m_nMaxCopsInPursuit(lua_State* L) { // уст. макс. копов в погоне
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {
			FindPlayerPed()->GetWanted()->m_nMaxCopsInPursuit = (int)lua_tointeger(L, 1);
		}
		else { throw "bad argument in function set_wanted_m_nMaxCopsInPursuit"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_wanted_m_nMaxCopCarsInPursuit(lua_State* L) { // макс. машин копов в погоне
	try {
		if (true) {
			lua_pushinteger(L, (lua_Integer)FindPlayerPed()->GetWanted()->m_nMaxCopCarsInPursuit);
			return 1;
		}
		else { throw "bad argument in function get_wanted_m_nMaxCopCarsInPursuit"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_wanted_m_nMaxCopCarsInPursuit(lua_State* L) { // уст. макс. машин копов в погоне
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {
			FindPlayerPed()->GetWanted()->m_nMaxCopCarsInPursuit = (int)lua_tointeger(L, 1);
		}
		else { throw "bad argument in function set_wanted_m_nMaxCopCarsInPursuit"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int get_wanted_m_nCopsBeatingSuspect(lua_State* L) { // копов избивают подозреваемого
	try {
		if (true) {
			lua_pushinteger(L, (lua_Integer)FindPlayerPed()->GetWanted()->m_nCopsBeatingSuspect);
			return 1;
		}
		else { throw "bad argument in function get_wanted_m_nCopsBeatingSuspect"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int choose_gang_ped_model(lua_State* L) { // выбрать модель педа банды
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {
			lua_pushinteger(L, (lua_Integer)CGangs::ChooseGangPedModel((short)lua_tointeger(L, 1)));
			return 1;
		}
		else { throw "bad argument in function choose_gang_ped_model"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int set_gang_attack_player_with_cops(lua_State* L) { // банда атакует игрока с копами
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TBOOLEAN == lua_type(L, 2)) {
			CGangs::SetWillAttackPlayerWithCops((ePedType)lua_tointeger(L, 1), (bool)lua_toboolean(L, 2));
		}
		else { throw "bad argument in function set_gang_attack_player_with_cops"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int init_gangs(lua_State* L) { // инициализировать банды
	CGangs::Initialise();
	return 0;};
int register_motion_streak(lua_State* L) { // размытие-полоса (id, левая/правая точки, rgb)
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4) &&
			LUA_TNUMBER == lua_type(L, 5) && LUA_TNUMBER == lua_type(L, 6) && LUA_TNUMBER == lua_type(L, 7) &&
			LUA_TNUMBER == lua_type(L, 8) && LUA_TNUMBER == lua_type(L, 9) && LUA_TNUMBER == lua_type(L, 10)) {
			CVector left = { (float)lua_tonumber(L, 2), (float)lua_tonumber(L, 3), (float)lua_tonumber(L, 4) };
			CVector right = { (float)lua_tonumber(L, 5), (float)lua_tonumber(L, 6), (float)lua_tonumber(L, 7) };
			CMotionBlurStreaks::RegisterStreak((unsigned int)lua_tointeger(L, 1), (unsigned char)lua_tointeger(L, 8), (unsigned char)lua_tointeger(L, 9), (unsigned char)lua_tointeger(L, 10), left, right);
		}
		else { throw "bad argument in function register_motion_streak"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;};
int init_trains(lua_State* L) { // инициализировать поезда
	CTrain::InitTrains();
	return 0;};
int update_trains(lua_State* L) { // обновить поезда
	CTrain::UpdateTrains();
	return 0;};
int shutdown_trains(lua_State* L) { // завершить поезда
	CTrain::Shutdown();
	return 0;};

int create_rope2(lua_State* L) { // создать веревку на координатах.
	//try {
/*
адрес 00632610
char __thiscall CRope::Render(int this)
{
  int v1; // ebx
  double v2; // st7
  double v3; // st6
  double v4; // st7
  int v5; // eax
  int v6; // edx
  signed int v7; // eax
  int v8; // ecx
  int v9; // esi
  int v10; // ebp
  int v11; // esi
  int v12; // ebp
  int v13; // esi
  int v14; // ebp
  int v15; // esi
  int v16; // ebp
  int v17; // esi
  int v18; // ebp
  int v19; // esi
  int v20; // ebp
  int v21; // esi
  int v22; // ebp
  int v23; // ecx
  float v25; // [esp+4h] [ebp-18h]
  float v26; // [esp+8h] [ebp-14h]
  float v27; // [esp+Ch] [ebp-10h]

  v1 = this;
  v2 = *(float *)(this + 208);
  v3 = *(float *)(this + 212);
  v25 = *(float *)(this + 204);
  v26 = v2;
  v27 = v3;
  RwV3dTransformPoints(&v25, &v25, 1, &unk_7E4EA8);
  v4 = v26;
  if ( v26 + flt_6DC384 < CDraw::ms_fNearClipZ )
  {
	if ( v4 - flt_6DC384 <= CDraw::ms_fFarClipZ )
	{
	  if ( v4 * flt_7E4F44 + v25 * flt_7E4F40 <= flt_6DC384 )
	  {
		if ( v4 * flt_7E4F50 + v25 * flt_7E4F4C <= flt_6DC384 )
		{
		  if ( v27 * flt_7E4F60 + v4 * flt_7E4F5C <= flt_6DC384 )
			LOBYTE(v5) = v27 * flt_7E4F6C + v4 * flt_7E4F68 <= flt_6DC384;
		  else
			LOBYTE(v5) = 0;
		}
		else
		{
		  LOBYTE(v5) = 0;
		}
	  }
	  else
	  {
		LOBYTE(v5) = 0;
	  }
	}
	else
	{
	  LOBYTE(v5) = 0;
	}
  }
  else
  {
	LOBYTE(v5) = 0;
  }
  if ( (_BYTE)v5 )
  {
	v6 = 0;
	v7 = 0;
	v8 = 0;
	do
	{
	  TempVertexBuffer[v6].color = 1686143104;
	  v9 = v7 + 1;
	  TempVertexBuffer[v6].objVertex.x = *(float *)(v1 + v8 + 12);
	  TempVertexBuffer[v6].objVertex.y = *(float *)(v1 + v8 + 16);
	  TempVertexBuffer[v6].objVertex.z = *(float *)(v1 + v8 + 20);
	  v8 += 96;
	  v6 += 8;
	  TempVertexBuffer[v9].color = 1686143104;
	  v10 = 12 * (v7 + 1);
	  TempVertexBuffer[v9].objVertex.x = *(float *)(v1 + v10 + 12);
	  TempVertexBuffer[v9].objVertex.y = *(float *)(v1 + v10 + 16);
	  TempVertexBuffer[v9].objVertex.z = *(float *)(v1 + v10 + 20);
	  v11 = v7 + 2;
	  TempVertexBuffer[v11].color = 1686143104;
	  v12 = 12 * (v7 + 2);
	  TempVertexBuffer[v11].objVertex.x = *(float *)(v1 + v12 + 12);
	  TempVertexBuffer[v11].objVertex.y = *(float *)(v1 + v12 + 16);
	  TempVertexBuffer[v11].objVertex.z = *(float *)(v1 + v12 + 20);
	  v13 = v7 + 3;
	  TempVertexBuffer[v13].color = 1686143104;
	  v14 = 12 * (v7 + 3);
	  TempVertexBuffer[v13].objVertex.x = *(float *)(v1 + v14 + 12);
	  TempVertexBuffer[v13].objVertex.y = *(float *)(v1 + v14 + 16);
	  TempVertexBuffer[v13].objVertex.z = *(float *)(v1 + v14 + 20);
	  v15 = v7 + 4;
	  TempVertexBuffer[v15].color = 1686143104;
	  v16 = 12 * (v7 + 4);
	  TempVertexBuffer[v15].objVertex.x = *(float *)(v1 + v16 + 12);
	  TempVertexBuffer[v15].objVertex.y = *(float *)(v1 + v16 + 16);
	  TempVertexBuffer[v15].objVertex.z = *(float *)(v1 + v16 + 20);
	  v17 = v7 + 5;
	  TempVertexBuffer[v17].color = 1686143104;
	  v18 = 12 * (v7 + 5);
	  TempVertexBuffer[v17].objVertex.x = *(float *)(v1 + v18 + 12);
	  TempVertexBuffer[v17].objVertex.y = *(float *)(v1 + v18 + 16);
	  TempVertexBuffer[v17].objVertex.z = *(float *)(v1 + v18 + 20);
	  v19 = v7 + 6;
	  TempVertexBuffer[v19].color = 1686143104;
	  v20 = 12 * (v7 + 6);
	  TempVertexBuffer[v19].objVertex.x = *(float *)(v1 + v20 + 12);
	  TempVertexBuffer[v19].objVertex.y = *(float *)(v1 + v20 + 16);
	  TempVertexBuffer[v19].objVertex.z = *(float *)(v1 + v20 + 20);
	  v21 = v7 + 7;
	  TempVertexBuffer[v21].color = 1686143104;
	  v22 = 4 * (v7 + 7);
	  v7 += 8;
	  v22 *= 3;
	  TempVertexBuffer[v21].objVertex.x = *(float *)(v1 + v22 + 12);
	  TempVertexBuffer[v21].objVertex.y = *(float *)(v1 + v22 + 16);
	  TempVertexBuffer[v21].objVertex.z = *(float *)(v1 + v22 + 20);
	}
	while ( v7 < 32 );
	RwRenderStateSet(12, 1);
	RwRenderStateSet(10, 5);
	RwRenderStateSet(11, 6);
	RwRenderStateSet(1, 0);
	v5 = RwIm3DTransform((int)TempVertexBuffer, 0x20u, 0, 24);
	if ( v5 )
	{
	  RwIm3DRenderIndexedPrimitive(2, &unk_6DC300, 62);
	  LOBYTE(v5) = RwIm3DEnd(v23);
	}
  }
  return v5;
}
*/
	return 0;
	//}// int
//}
//	catch (const char* x) { writelog(x); }// записать ошибку в файл.
//	return 0;
};

int create_rope3(lua_State* L) { // создать веревку на координатах.
	//try {
/*
адрес 006329A0

__int16 __thiscall CRope::Update(float *this)
{
  long double v1; // st2
  float *v2; // ebx
  long double v3; // st2
  float v6; // ST18_4
  double v7; // st7
  double v8; // st5
  double v9; // st6
  signed int v10; // ecx
  signed int v11; // eax
  float v12; // ST0C_4
  float v13; // ST10_4
  float v14; // ST14_4
  double v15; // st5
  double v16; // st6
  double v17; // st2
  double v18; // st1
  double v19; // st2
  double v20; // st1
  float v21; // ST18_4
  double v22; // st6
  double v23; // st5
  float v24; // ST08_4
  double v25; // st4
  float v26; // ST04_4
  double v27; // st3
  float v28; // ST00_4
  double v29; // st4
  double v30; // st5
  double v31; // st6
  __int16 v32; // fps
  double v33; // st2
  bool v34; // c0
  char v35; // c2
  bool v36; // c3

  v1 = CTimer::ms_fTimeStep;
  v2 = this;
  if ( dbl_6DC388 <= 0.0 )
  {
	powf(0.0);
  }
  else
  {
	v3 = __FYL2X__(dbl_6DC388, v1);
	_ST1 = v3;
	__asm { frndint }
	v1 = __F2XM1__(v3 - _ST1) + flt_6DC39C;
  }
  v6 = v1;
  v7 = v6;
  if ( !*((_BYTE *)v2 + 1) && (unsigned int)CTimer::m_snTimeInMilliseconds > *((_DWORD *)v2 + 2) )
  {
	v2[101] = v2[101] - flt_6DC390 * CTimer::ms_fTimeStep;
	v8 = CTimer::ms_fTimeStep * v2[101];
	v9 = CTimer::ms_fTimeStep * v2[100];
	v2[3] = CTimer::ms_fTimeStep * v2[99] + v2[3];
	v2[4] = v2[4] + v9;
	v2[5] = v2[5] + v8;
  }
  v10 = 3;
  v11 = 1;
  do
  {
	v12 = v2[v10 + 3];
	v13 = v2[v10 + 4];
	v14 = v2[v10 + 5];
	v15 = CTimer::ms_fTimeStep * v2[v10 + 101] * v7;
	v16 = CTimer::ms_fTimeStep * v2[v10 + 100] * v7;
	v2[v10 + 3] = CTimer::ms_fTimeStep * v2[v10 + 99] * v7 + v12;
	v2[v10 + 4] = v2[v10 + 4] + v16;
	v2[v10 + 5] = v2[v10 + 5] + v15;
	v2[v10 + 5] = v2[v10 + 5] - flt_6DC394 * CTimer::ms_fTimeStep;
	v17 = v2[v10 + 4] - v2[v10 + 1];
	v18 = v2[v10 + 3] - v2[v10];
	v19 = v17 * v17 + v18 * v18;
	v20 = v2[v10 + 5] - v2[v10 + 2];
	v21 = sqrt(v19 + v20 * v20);
	v22 = flt_6DC398 / v21;
	v23 = v2[v10 + 2];
	v24 = v2[v10 + 5] - v23;
	v25 = v2[v10 + 1];
	v26 = v2[v10 + 4] - v25;
	v27 = v2[v10];
	v28 = v2[v10 + 3] - v27;
	++v11;
	v2[v10 + 3] = v22 * v28 + v27;
	v2[v10 + 4] = v22 * v26 + v25;
	v2[v10 + 5] = v22 * v24 + v23;
	v29 = flt_6DC39C / CTimer::ms_fTimeStep;
	v30 = (v2[v10 + 5] - v14) * v29;
	v31 = (v2[v10 + 4] - v13) * v29;
	v2[v10 + 99] = (v2[v10 + 3] - v12) * v29;
	v2[v10 + 100] = v31;
	v2[v10 + 101] = v30;
	v10 += 3;
  }
  while ( v11 < 32 );
  if ( !*((_BYTE *)v2 + 1) )
  {
	v33 = v2[5];
	v34 = v33 < flt_6DC3A0;
	v35 = 0;
	v36 = v33 == flt_6DC3A0;
	LOWORD(v11) = __PAIR__(HIBYTE(v32), (unsigned __int8)v32) & 0x5FF;
	if ( v33 >= flt_6DC3A0 )
	  *(_BYTE *)v2 = 0;
  }
  *((_BYTE *)v2 + 1) = 0;
  return v11;
}
*/
	return 0;
	//}// int
//}
//	catch (const char* x) { writelog(x); }// записать ошибку в файл.
//	return 0;
};

int create_rope4(lua_State* L) { // создать веревку на координатах.
	//try {
/*
адрес 00632C00
char __cdecl CRopes::FindCoorsAlongRope(int a1, float a2, int a3)
{
  signed int v3; // ecx
  int v4; // edx
  double v5; // st2
  double v6; // st7
  int v7; // ecx
  int v8; // eax
  int v9; // edx
  double v10; // st7
  double v11; // st5
  double v12; // st3
  double v13; // st4
  double v14; // st6
  double v15; // st1
  char result; // al

  v3 = 0;
  v4 = 0;
  while ( !byte_936918[v4 * 4] || a1 != dword_93691C[v4] )
  {
	++v3;
	v4 += 195;
	if ( v3 >= 8 )
	  return 0;
  }
  if ( flt_6DC3A0 <= (double)a2 )
	v5 = a2;
  else
	v5 = flt_6DC3A0;
  if ( flt_6DC3A4 >= v5 )
	v5 = flt_6DC3A4;
  v6 = flt_6DC3A8 * v5;
  v7 = 65 * v3;
  v8 = 3 * (v7 + (signed int)v6);
  v9 = 3 * (v7 + (signed int)v6 + 1);
  v10 = v6 - (double)(signed int)v6;
  v11 = v10 * flt_93692C[v9];
  v12 = flt_6DC39C - v10;
  v13 = v12 * flt_93692C[v8];
  v14 = v10 * flt_936928[v9] + v12 * flt_936928[v8];
  v15 = v12 * flt_936924[v8];
  result = 1;
  *(float *)a3 = v10 * flt_936924[v9] + v15;
  *(float *)(a3 + 4) = v14;
  *(float *)(a3 + 8) = v13 + v11;
  return result;
}

*/
	return 0;
	//}// int
//}
//	catch (const char* x) { writelog(x); }// записать ошибку в файл.
//	return 0;
};
int create_rope5(lua_State* L) { // создать веревку на координатах.
	//try {
/*
адрес 00632D50

int __cdecl CRopes::SetSpeedOfTopNode(int a1, int a2, int a3, int a4)
{
  signed int v4; // edx
  int result; // eax
  int v6; // ecx
  int v7; // edx

  v4 = 0;
  result = a1;
  v6 = 0;
  while ( !byte_936918[v6 * 4] || a1 != dword_93691C[v6] )
  {
	++v4;
	v6 += 195;
	if ( v4 >= 8 )
	  return result;
  }
  v7 = 195 * v4;
  dword_936AA4[v7] = a2;
  dword_936AA8[v7] = a3;
  dword_936AAC[v7] = a4;
  return result;
}
*/
	return 0;
	//}// int
//}
//	catch (const char* x) { writelog(x); }// записать ошибку в файл.
//	return 0;
};
int create_rope6(lua_State* L) { // создать веревку на координатах.
	//try {
/*
адрес 00632DB0
char __cdecl CRopes::RegisterRope(int a1, float a2, float a3, float a4, char a5)
{
  signed int v5; // ebx
  int v6; // eax
  int v7; // ebx
  char result; // al
  signed int v9; // eax
  int v10; // ecx
  int v11; // ecx
  signed int v12; // ebx
  int v13; // eax
  int v14; // edx
  double v15; // st6
  double v16; // st7
  int v17; // edx

  v5 = 0;
  v6 = 0;
  do
  {
	if ( byte_936918[v6 * 4] && a1 == dword_93691C[v6] )
	{
	  v7 = 195 * v5;
	  result = 1;
	  flt_936924[v7] = a2;
	  flt_936928[v7] = a3;
	  flt_93692C[v7] = a4;
	  dword_936AA4[v7] = 0;
	  dword_936AA8[v7] = 0;
	  dword_936AAC[v7] = 0;
	  *((_BYTE *)&unk_936919 + v7 * 4) = 1;
	  return result;
	}
	++v5;
	v6 += 195;
  }
  while ( v5 < 8 );
  v9 = 0;
  v10 = 0;
  while ( byte_936918[v10] )
  {
	++v9;
	v10 += 780;
	if ( v9 >= 8 )
	  return 0;
  }
  v11 = 195 * v9;
  dword_93691C[v11] = a1;
  flt_936924[v11] = a2;
  flt_936928[v11] = a3;
  flt_93692C[v11] = a4;
  dword_936AA4[v11] = 0;
  dword_936AA8[v11] = 0;
  dword_936AAC[v11] = 0;
  byte_93691A[v11 * 4] = 0;
  *((_BYTE *)&unk_936919 + v11 * 4) = 1;
  if ( a5 )
	dword_936920[v11] = CTimer::m_snTimeInMilliseconds + 20000;
  else
	dword_936920[v11] = 0;
  v12 = 1;
  v13 = 65 * v9 + 1;
  do
  {
	if ( v12 & 1 )
	{
	  v14 = 3 * v13;
	  v15 = *(float *)&dword_936920[3 * v13] + flt_6DC3A0;
	  v16 = *(float *)&dword_93691C[3 * v13] + flt_6DC3A0;
	  flt_936924[3 * v13] = *(float *)&byte_936918[12 * v13] + flt_6DC398;
	}
	else
	{
	  v14 = 3 * v13;
	  v15 = *(float *)&dword_936920[3 * v13] - flt_6DC3A0;
	  v16 = *(float *)&dword_93691C[3 * v13] - flt_6DC3A0;
	  flt_936924[3 * v13] = *(float *)&byte_936918[12 * v13] - flt_6DC398;
	}
	flt_936928[v14] = v16;
	flt_93692C[v14] = v15;
	++v12;
	v17 = 4 * v13++;
	v17 *= 3;
	*(int *)((char *)dword_936AA4 + v17) = 0;
	*(int *)((char *)dword_936AA8 + v17) = 0;
	*(int *)((char *)dword_936AAC + v17) = 0;
  }
  while ( v12 < 32 );
  byte_936918[v11 * 4] = 1;
  return 1;
}
*/
	return 0;
	//}// int
//}
//	catch (const char* x) { writelog(x); }// записать ошибку в файл.
//	return 0;
};
int create_rope7(lua_State* L) { // создать веревку на координатах.
	//try {
/*
адрес 00632DB0

char CRopes::Render()
{
  signed int v0; // ebx
  int v1; // ebp
  char *v2; // esi
  char result; // al

  v0 = 0;
  v1 = 0;
  v2 = byte_936918;
  do
  {
	if ( byte_936918[v1] )
	  result = CRope::Render((int)v2);
	++v0;
	v1 += 780;
	v2 += 780;
  }
  while ( v0 < 8 );
  return result;
}
*/
	return 0;
	//}// int
//}
//	catch (const char* x) { writelog(x); }// записать ошибку в файл.
//	return 0;
};
int create_rope8(lua_State* L) { // создать веревку на координатах.
	//try {
/*
адрес 00633000
__int16 __cdecl CRopes::Update()
{
  signed int v0; // ebx
  int v1; // ebp
  float *v2; // esi
  __int16 result; // ax

  v0 = 0;
  v1 = 0;
  v2 = (float *)byte_936918;
  do
  {
	if ( byte_936918[v1] )
	  result = CRope::Update(v2);
	++v0;
	v1 += 780;
	v2 += 195;
  }
  while ( v0 < 8 );
  return result;
}
*/
	return 0;
	//}// int
//}
//	catch (const char* x) { writelog(x); }// записать ошибку в файл.
//	return 0;
};

int create_rope9(lua_State* L) { // создать веревку на координатах.
	//try {
/*
адрес 00633040

void __cdecl CRopes::Init()
{
  byte_936918[0] = 0;
  byte_936C24 = 0;
  byte_936F30 = 0;
  byte_93723C = 0;
  byte_937548 = 0;
  byte_937854 = 0;
  byte_937B60 = 0;
  byte_937E6C = 0;
}
*/
	return 0;
	//}// int
//}
//	catch (const char* x) { writelog(x); }// записать ошибку в файл.
//	return 0;
};
int create_rope10(lua_State* L) { // создать веревку на координатах.
	//try {
/*
адрес 00633080
int CRopes_CRopes()
{
  return _construct_array((int)byte_936918, (int (__thiscall *)(int, signed int))CRopes_DCRopes, 0, 780, 8u);
}
*/
	return 0;
	//}// int
//}
//	catch (const char* x) { writelog(x); }// записать ошибку в файл.
//	return 0;
};
int create_rope11(lua_State* L) { // создать веревку на координатах.
	//try {
/*
адрес 006330A0

void *__thiscall CRopes_DCRopes(void *this)
{
  void *v1; // ebx

  v1 = this;
  _construct_array((int)this + 12, (int (__thiscall *)(int, signed int))CVector_CVector, 0, 12, 0x20u);
  _construct_array((int)v1 + 396, (int (__thiscall *)(int, signed int))CVector_CVector, 0, 12, 0x20u);
  return v1;
}
*/
	return 0;
	//}// int
//}
//	catch (const char* x) { writelog(x); }// записать ошибку в файл.
//	return 0;
};
int turncar(CVehicle* car, float angle, float speed, int switc) {

	speed = speed / 360;
	while (true) {

		this_thread::sleep_for(chrono::milliseconds(1));

		if (switc == 1) {
			car->m_vecFrictionTurnForce.x = speed;
			car->m_vecTurnSpeed.x = speed;
			CVector* anl = get_car_angles_on_axes(car);
			if (anl->x >= angle) {// угол по оси x.
				break;
			}
		}
		if (switc == 2) {
			car->m_vecFrictionTurnForce.y = speed;
			car->m_vecTurnSpeed.y = speed;
			CVector* anl = get_car_angles_on_axes(car);
			if (anl->y >= angle) {// угол по оси y.
				break;
			}
		}
		if (switc == 3) {
			car->m_vecFrictionTurnForce.z = speed;
			car->m_vecTurnSpeed.z = speed;
			CVector* anl = get_car_angles_on_axes(car);
			if (anl->z >= angle) {// угол по оси z.
				break;
			}
		}
	};
	return 0;
};
int RotationMatrixToEulerianAngle(float* RotMx, vector<float>& carang);
int EulerianAngleNormalize360(vector<float>& EulAng);
int get_car_angle(CVehicle* car, vector<float>& carang) {

	float car_martix[9];
	car_martix[0] = car->right.x;
	car_martix[1] = car->right.y;
	car_martix[2] = car->right.z;
	car_martix[3] = car->up.x;
	car_martix[4] = car->up.y;
	car_martix[5] = car->up.z;
	car_martix[6] = car->at.x;
	car_martix[7] = car->at.y;
	car_martix[8] = car->at.z;
	RotationMatrixToEulerianAngle(car_martix, carang);
	EulerianAngleNormalize360(carang);

	return 0;
};
int EulerianAngleToRotationMatrix(vector<float>& EulAng, vector<float>& RotMx) {
	double cosx = cos(EulAng[0] / 57.295776);
	double sinx = sin(EulAng[0] / 57.295776);
	double cosy = cos(EulAng[1] / 57.295776);
	double siny = sin(EulAng[1] / 57.295776);
	double cosz = cos(EulAng[2] / 57.295776);
	double sinz = sin(EulAng[2] / 57.295776);
	RotMx[3 * 0 + 0] = (float)(cosz * cosy - sinz * sinx * siny);
	RotMx[3 * 0 + 1] = (float)(cosz * sinx * siny + sinz * cosy);
	RotMx[3 * 0 + 2] = (float)(-cosx * siny);
	RotMx[3 * 1 + 0] = (float)(-sinz * cosx);
	RotMx[3 * 1 + 1] = (float)(cosz * cosx);
	RotMx[3 * 1 + 2] = (float)(sinx);
	RotMx[3 * 2 + 0] = (float)(sinz * sinx * cosy + cosz * siny);
	RotMx[3 * 2 + 1] = (float)(sinz * siny - cosz * sinx * cosy);
	RotMx[3 * 2 + 2] = (float)(cosx * cosy);

	return 0;
};


int EulerianAngleNormalize360(vector<float>& EulAng) {
	if (EulAng[0] < 0) EulAng[0] = EulAng[0] + 360;
	if (EulAng[0] >= 360) EulAng[0] = EulAng[0] - 360;
	if (EulAng[1] < 0) EulAng[1] = EulAng[1] + 360;
	if (EulAng[1] >= 360) EulAng[1] = EulAng[1] - 360;
	if (EulAng[2] < 0) EulAng[2] = EulAng[2] + 360;
	if (EulAng[2] >= 360) EulAng[2] = EulAng[2] - 360;
	return 0;

};

int RotationMatrixToEulerianAngle(float* RotMx, vector<float>& carang) {
	//double rm00 = RotMx[3*0+0];
	//double rm01 = RotMx[3*0+1];
	double rm02 = RotMx[3 * 0 + 2];
	double rm10 = RotMx[3 * 1 + 0];
	double rm11 = RotMx[3 * 1 + 1];
	double rm12 = RotMx[3 * 1 + 2];
	//double rm20 = RotMx[3*2+0];
	//double rm21 = RotMx[3*2+1];
	double rm22 = RotMx[3 * 2 + 2];
	carang[2] = (float)(-atan2(rm10, rm11) * 57.295776);
	carang[0] = (float)(-asin(-rm12) * 57.295776);
	carang[1] = (float)(-atan2(rm02, rm22) * 57.295776);
	return 0;
};

int turncar_with_delay(CVehicle* car, float angle, int time, int switc) {

	while (true) {

		this_thread::sleep_for(chrono::milliseconds(time));
		float speed = 0.001;
		if (switc == 1) {
			car->m_vecFrictionTurnForce.x = speed;
			car->m_vecTurnSpeed.x = speed;
			CVector* anl = get_car_angles_on_axes(car);
			if (anl->x >= angle) {// угол по оси x.
				break;
			}
		}
		if (switc == 2) {
			car->m_vecFrictionTurnForce.y = speed;
			car->m_vecTurnSpeed.y = speed;
			CVector* anl = get_car_angles_on_axes(car);
			if (anl->y >= angle) {// угол по оси y.
				break;
			}
		}
		if (switc == 3) {
			car->m_vecFrictionTurnForce.z = speed;
			car->m_vecTurnSpeed.z = speed;
			CVector* anl = get_car_angles_on_axes(car);
			if (anl->z >= angle) {// угол по оси z.
				break;
			}
		}
	};
	return 0;
};

struct corona {// структура для короны.
	static bool switc;
	static float abc_x; static float ord_y; static float pos_z; static float radius;
	static int type; static int glow_flare; static int red; static int blue;
	static int green;

	static void set(bool switc1, float abc_x1, float ord_y1, float pos_z1, float radius1,
		int type1, int glow_flare1, int red1, int blue1, int green1) { // установить цвет короны.
		switc = switc1;
		abc_x = abc_x1; ord_y = ord_y1;	pos_z = pos_z1; type = type1; radius = radius1; glow_flare = glow_flare1;
		red = red1; blue = blue1; green = green1;
	}
	static void draw() {// вывод короны.
		float x = abc_x;	float y = ord_y;	float z = pos_z;
		float radius1 = radius; int type1 = type;	int glow_flare1 = glow_flare;
		int red1 = red;  int blue1 = blue; int green1 = green;
		if (switc == true) {
			Command<COMMAND_DRAW_CORONA>(x, y, z, radius1, type1, glow_flare1, red1, green1, blue1);
		}
	}//;
};

int destroy(lua_State* L) {// удаления объектов из памяти при перезагрузки скрипта.
 	cpp_trace("destroy: ВХОД");
 	int i;
 	map<int, lua_State*>::iterator it;

 	for (auto it = markeron.begin(); it != markeron.end();) {
 		if (L == it->second) {
 			i = it->first;
 			Command<COMMAND_REMOVE_BLIP>(i);//удалить маркер.
 			it = markeron.erase(it);
 		}
 		else { ++it; }
 	}

 	for (auto it = spheres.begin(); it != spheres.end();) {
 		if (L == it->second) {
 			i = it->first;
 			Command<COMMAND_REMOVE_SPHERE>(i);// удалить сферу.
 			it = spheres.erase(it);
 		}
 		else { ++it; }
 	}

 	//map<int, lua_State*>::iterator pick;

 	for (auto pick = pickupsids.begin(); pick != pickupsids.end();) {
 		if (L == pick->second) {
 			i = pick->first;
 			Command<COMMAND_REMOVE_PICKUP>(i);// удалить пикап.
 			pick = pickupsids.erase(pick);
 		}
 		else { ++pick; }
 	}
 	for (auto it = firesids.begin(); it != firesids.end();) {
 		if (L == it->second) {
 			i = it->first;
 			Command<COMMAND_REMOVE_SCRIPT_FIRE>(i);// удалить огонь.
 			it = firesids.erase(it);
 		}
 		else { ++it; }
 	}
 	for (auto it = soundsids.begin(); it != soundsids.end();) {
 		if (L == it->second) {
 			i = it->first;
 			Command<COMMAND_REMOVE_SOUND>(i);// удалить непрерывный звук.
 			it = soundsids.erase(it);
 		}
 		else { ++it; }
 	}

 	// сначала авто (выкидывая из них сидящих), потом педов — иначе удаление
 	// авто с сидящим внутри игроком/педом роняет игру.
 	for (auto cars = mapcars.begin(); cars != mapcars.end();) {
 		if (L == cars->second) {
 			cpp_tracef("destroy: удаляю авто %p", (void*)cars->first);
 			safe_remove_car(cars->first);// безопасно удалить авто (с проверкой указателя).
 			cars = mapcars.erase(cars);
 		}
 		else { ++cars; }
 	}

 	for (auto pick = mappeds.begin(); pick != mappeds.end();) {
 		if (L == pick->second) {
 			CPed* ped = pick->first;
 			cpp_tracef("destroy: удаляю педа %p", (void*)ped);
 			if (findpedinpool(ped) == NULL) { cpp_trace("destroy: пед висячий, просто стираю запись"); pick = mappeds.erase(pick); continue; }// висячий — стереть запись.
 			warp_ped_out_of_car(ped);// пед сидит в авто? высадить перед удалением.
 			ped->ClearInvestigateEvent();// пед уходит, опустить педа.

 			ped->ClearObjective(); // снять задачи с педа.
 			Command<COMMAND_MARK_CHAR_AS_NO_LONGER_NEEDED>(CPools::GetPedRef(ped));// удалить педа.
 			CEntity* p1 = (CEntity*)ped;
 			if (p1 != NULL) {//obj->Remove();
 				CWorld::Remove(p1);
 			}
 			pick = mappeds.erase(pick);
 		}
 		else { ++pick; }
 	}

 	for (auto objs = mapobjs.begin(); objs != mapobjs.end();) {
 		if (L == objs->second) {
 			CObject* obj = objs->first;
 			if (findobjinpool(obj) == NULL) { objs = mapobjs.erase(objs); continue; }// висячий — стереть запись.
 			Command<COMMAND_DELETE_OBJECT>(CPools::GetObjectRef(obj));// удалить объект.
 			CEntity* p1 = (CEntity*)obj;
 			if (p1 != NULL) {//obj->Remove();
 				CWorld::Remove(p1);
 			}
 			objs = mapobjs.erase(objs);
 		}
 		else { ++objs; }
 	}
 	bool s = false;
 	corona::set(s, 0, 0, 0, 0, 0, 0, 0, 0, 0);
 	cpp_trace("destroy: ВЫХОД");
 	return 0;
 };

CPed* findpedinpool(const void* p) {// найти педа в пуле.
	for (auto ped : CPools::ms_pPedPool) {
		if (ped == p) {
			this_thread::sleep_for(chrono::milliseconds(1));
			return ped;
		}
	};
	CPed* ped2 = NULL;
	return ped2;
};

CVector* get_car_angles_on_axes(CVehicle* car) {

	CVector* anl;
	float car_martix[9];
	car_martix[0] = car->right.x;
	car_martix[1] = car->right.y;
	car_martix[2] = car->right.z;
	car_martix[3] = car->up.x;
	car_martix[4] = car->up.y;
	car_martix[5] = car->up.z;
	car_martix[6] = car->at.x;
	car_martix[7] = car->at.y;
	car_martix[8] = car->at.z;
	//double rm00 = RotMx[3*0+0];
//double rm01 = RotMx[3*0+1];
	double rm02 = car_martix[3 * 0 + 2];
	double rm10 = car_martix[3 * 1 + 0];
	double rm11 = car_martix[3 * 1 + 1];
	double rm12 = car_martix[3 * 1 + 2];
	//double rm20 = RotMx[3*2+0];
	//double rm21 = RotMx[3*2+1];
	double rm22 = car_martix[3 * 2 + 2];
	anl->z = (float)(-atan2(rm10, rm11) * 57.295776);
	anl->x = (float)(-asin(-rm12) * 57.295776);
	anl->y = (float)(-atan2(rm02, rm22) * 57.295776);
	if (anl->x < 0) anl->x = anl->x + 360;
	if (anl->x >= 360) anl->x = anl->x - 360;
	if (anl->y < 0) anl->y = anl->y + 360;
	if (anl->y >= 360) anl->y = anl->y - 360;
	if (anl->z < 0) anl->z = anl->z + 360;
	if (anl->z >= 360) anl->z = anl->z - 360;

	return anl;
};

CVehicle* findcarinpool(const void* p) {// найти авто в пуле.
	for (auto car : CPools::ms_pVehiclePool) {
		if (car == p) {
			this_thread::sleep_for(chrono::milliseconds(1));
			return car;
		}
	};
	CVehicle* car2 = NULL;
	return car2;
};
CObject* findobjinpool(const void* p) {// найти объект в пуле.
	for (auto obj : CPools::ms_pObjectPool) {
		if (obj == p) {
			this_thread::sleep_for(chrono::milliseconds(1));
			return obj;
		}
	};
	CObject* obj2 = NULL;
	return obj2;
};

// выкинуть всех сидящих в авто (игрок, водитель, пассажиры) — перед удалением авто.
void warp_out_car_occupants(CVehicle* car) {
	if (car == NULL) return;
	CVector p = car->pos;// координаты авто — куда сажать выходящих.
	CPed* player = FindPlayerPed();// томми может сидеть в этом авто.
	if (player != NULL && player->m_pVehicle == car && player->m_fHealth > 0.0f) {
		Command<COMMAND_WARP_CHAR_FROM_CAR_TO_COORD>(CPools::GetPedRef(player), p.x + 1.0f, p.y, p.z);
	}
	if (car->m_pDriver != NULL && car->m_pDriver->m_fHealth > 0.0f) {
		Command<COMMAND_WARP_CHAR_FROM_CAR_TO_COORD>(CPools::GetPedRef(car->m_pDriver), p.x + 1.0f, p.y, p.z);
	}
	for (int i = 0; i < 8; i++) {// пассажиры (8 мест).
		CPed* pass = car->m_passengers[i];
		if (pass != NULL && pass->m_fHealth > 0.0f) {
			Command<COMMAND_WARP_CHAR_FROM_CAR_TO_COORD>(CPools::GetPedRef(pass), p.x + 1.0f, p.y, p.z);
		}
	}
};

// выкинуть одного педа из авто, если он там сидит — перед удалением педа.
void warp_ped_out_of_car(CPed* ped) {
	if (ped == NULL) return;
	if (!ped->m_bInVehicle || ped->m_pVehicle == NULL) return;// пед не в авто.
	CVehicle* car = ped->m_pVehicle;
	if (findcarinpool(car) == NULL) return;// авто уже удалено движком.
	CVector p = car->pos;// координаты авто — куда сажать выходящего.
	if (ped->m_fHealth > 0.0f) {
		Command<COMMAND_WARP_CHAR_FROM_CAR_TO_COORD>(CPools::GetPedRef(ped), p.x + 1.0f, p.y, p.z);
	}
};

// безопасно удалить авто: выкинуть сидящих, пометить ненужным, убрать из мира.
void safe_remove_car(CVehicle* car) {
	if (car == NULL) return;
	if (findcarinpool(car) == NULL) return;// авто уже удалено движком (висячий указатель).
	warp_out_car_occupants(car);// выкинуть из авто всех сидящих (включая игрока).
	Command<COMMAND_MARK_CAR_AS_NO_LONGER_NEEDED>(CPools::GetVehicleRef(car));
	CWorld::Remove((CEntity*)car);
};
int find_in_map(std::map<string, int>& carlist, const char* search) {

	auto it = carlist.find(search);
	if (it == carlist.end()) {
		return 0;
	}
	else {
		int m = it->second;
		return m;
	}
};
int find_model_in_map(std::map<int, int>& type_and_model, int search) {

	auto it = type_and_model.find(search);
	if (it == type_and_model.end()) {
		return 0;
	}
	else {
		int m = it->second;
		return m;
	}
};
void showstack(lua_State* L) {
	int i = lua_gettop(L);/* получаем количество элементов в стеке.*/
	string path = "stack.txt";
	fstream f2; {f2.open("stack.txt", fstream::in | fstream::out | fstream::app);
	if (f2.is_open()) {// если файл есть удалить.
		f2.close();	remove("stack.txt");
	}}
	fstream f1; {f1.open(path, fstream::in | fstream::out | fstream::app);
	int j = (i) * -1 - 1;
	i = -1;
	for (i; i > j; i--) {
		int t = lua_type(L, i);
		if (LUA_TSTRING == t) {
			f1 << "str ";
			f1 << i; f1 << "\n";
		}
		if (LUA_TNUMBER == t) {
			f1 << "number ";
			f1 << i; f1 << "\n";
		}

		if (LUA_TBOOLEAN == t) {
			f1 << "LUA_TBOOLEAN ";
			f1 << i; f1 << "\n";
		}
		if (LUA_TLIGHTUSERDATA == t) {
			f1 << "LIGHTUSERDATA ";
			f1 << i; f1 << "\n";
		}
		if (LUA_TTABLE == t) {
			f1 << "LUA_TTABLE ";
			f1 << i; f1 << "\n";
		}
		if (LUA_TFUNCTION == t) {

			f1 << "funs ";
			f1 << i; f1 << "\n";
		}
		if (LUA_TLIGHTUSERDATA == t) {
			f1 << "user ";
			f1 << i; f1 << "\n";
		}
		if (LUA_TTHREAD == t) {
			f1 << "LUA_TTHREAD "; f1 << "\n";
		}
		if (LUA_TNIL == t) {
			f1 << "LUA_TNIL "; f1 << "\n";
		}
	}
	f1.close(); }
};

void showstack1(lua_State* L) {
	int i = lua_gettop(L);/* получаем количество элементов в стеке.*/
	string path = "stack1.txt";
	fstream f1; {f1.open(path, fstream::in | fstream::out | fstream::app);
	int j = (i) * -1 - 1;
	i = -1;
	for (i; i > j; i--) {
		int t = lua_type(L, i);
		if (LUA_TSTRING == t) {
			f1 << "str ";
			f1 << i; f1 << "\n";
		}
		if (LUA_TNUMBER == t) {
			f1 << "number ";
			f1 << i; f1 << "\n";
		}

		if (LUA_TBOOLEAN == t) {
			f1 << "LUA_TBOOLEAN ";
			f1 << i; f1 << "\n";
		}
		if (LUA_TLIGHTUSERDATA == t) {
			f1 << "LIGHTUSERDATA ";
			f1 << i; f1 << "\n";
		}
		if (LUA_TTABLE == t) {
			f1 << "LUA_TTABLE ";
			f1 << i; f1 << "\n";
		}
		if (LUA_TFUNCTION == t) {
			f1 << "funs ";
			f1 << i; f1 << "\n";
		}
		if (LUA_TLIGHTUSERDATA == t) {
			f1 << "user ";
			f1 << i; f1 << "\n";
		}
		if (LUA_TTHREAD == t) {
			f1 << "LUA_TTHREAD "; f1 << "\n";
		}
		if (LUA_TNIL == t) {
			f1 << "LUA_TNIL "; f1 << "\n";
		}
	}
	f1.close(); }
};// кол-во аргументов.
void reversestack(lua_State* L) {// реверс стека.
	int stacksize = lua_gettop(L);
	stacksize++;
	for (int i = 1; i < stacksize; i++) { lua_insert(L, i); }
};
string getkey(int key) {

	if (GetAsyncKeyState(0x30)) {
		return "0";
	}
	if (GetAsyncKeyState(0x31)) {
		return "1";
	}
	if (GetAsyncKeyState(0x32)) {
		return "2";
	}
	if (GetAsyncKeyState(0x33)) {
		return "3";
	}
	if (GetAsyncKeyState(0x34)) {
		return "4";
	}
	if (GetAsyncKeyState(0x35)) {
		return "5";
	}
	if (GetAsyncKeyState(0x36)) {
		return "6";
	}
	if (GetAsyncKeyState(0x37)) {
		return "7";
	}
	if (GetAsyncKeyState(0x38)) {
		return "8";
	}
	if (GetAsyncKeyState(0x39)) {
		return "9";
	}
	if (GetAsyncKeyState(0x41)) {
		return "a";
	}
	if (GetAsyncKeyState(0x42)) {
		return "b";
	}
	if (GetAsyncKeyState(0x43)) {
		return "c";
	}
	if (GetAsyncKeyState(0x44)) {
		return "d";
	}
	if (GetAsyncKeyState(0x45)) {
		return "e";
	}
	if (GetAsyncKeyState(0x46)) {
		return "f";
	}
	if (GetAsyncKeyState(0x47)) {
		return "g";
	}
	if (GetAsyncKeyState(0x48)) {
		return "h";
	}
	if (GetAsyncKeyState(0x49)) {
		return "i";
	}
	if (GetAsyncKeyState(0x4a)) {
		return "j";
	}
	if (GetAsyncKeyState(0x4b)) {
		return "k";
	}
	if (GetAsyncKeyState(0x4c)) {
		return "l";
	}
	if (GetAsyncKeyState(0x4d)) {
		return "m";
	}
	if (GetAsyncKeyState(0x4e)) {
		return "n";
	}
	if (GetAsyncKeyState(0x4f)) {
		return "o";
	}
	if (GetAsyncKeyState(0x50)) {
		return "p";
	}
	if (GetAsyncKeyState(0x51)) {
		return "q";
	}
	if (GetAsyncKeyState(0x52)) {
		return "r";
	}
	if (GetAsyncKeyState(0x53)) {
		return "s";
	}
	if (GetAsyncKeyState(0x54)) {
		return "t";
	}
	if (GetAsyncKeyState(0x55)) {
		return "u";
	}
	if (GetAsyncKeyState(0x56)) {
		return "v";
	}
	if (GetAsyncKeyState(0x57)) {
		return "w";
	}
	if (GetAsyncKeyState(0x58)) {
		return "x";
	}
	if (GetAsyncKeyState(0x59)) {
		return "y";
	}
	if (GetAsyncKeyState(0x5a)) {
		return "z";
	}
	if (GetAsyncKeyState(0x0d)) {
		return "\n";
	}
	if (GetAsyncKeyState(0x20)) {
		return " ";
	}
	return "";
};
int newthread(lua_State* L) {// новый поток.
	try {
		if (LUA_TFUNCTION == lua_type(L, 1)) {
			int stacksize = lua_gettop(L);
			//lua_pushinteger(L, stacksize);
			return lua_yield(L, stacksize);
		}
		else { throw "bad argument in function newthread"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

wchar_t* getwchat(const char* c) {// перевод в строку.
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize]; mbstowcs(wc, c, cSize);
	return wc;// вернуть строку.
};


bool getstatusmission() {// проверка флага миссии.
	unsigned int& OnAMissionFlag = *(unsigned int*)0x978748;
	bool getflagmission = (CTheScripts::ScriptSpace[OnAMissionFlag]);// получить флаг миссии.
	return getflagmission;
};

int setstatusmission(bool flag) { // уcтановить флага миссии.
	unsigned int& OnAMissionFlag = *(unsigned int*)0x978748;
	CTheScripts::ScriptSpace[OnAMissionFlag] = flag;
	return 0;
};

void writelog(const char x[]) {// запись ошибок в файл.

	mtx.lock();
	string path = "lualoader\\log.txt";// куда пишем ошибки.
	fstream f1; {f1.open(path, fstream::in | fstream::out | fstream::app);
	f1 << x; time_t rawtime; struct tm* timeinfo;
	char buffer[120]; time(&rawtime); timeinfo = localtime(&rawtime);
	strftime(buffer, sizeof(buffer), " %d-%m-%Y %I:%M:%S ", timeinfo);// датируем загрузки скриптов.
	string er2(buffer); f1 << er2 << "\n"; }
	mtx.unlock();
	f1.close();
};
 
void cpp_trace(const char x[]) {// запись трейса C++ в тот же trace.txt, что и ma_trace из lua.

	mtx.lock();
	string path = "lualoader\\trace.txt";// куда пишем трейс.
	fstream f1; {f1.open(path, fstream::in | fstream::out | fstream::app);
	f1 << "C++| " << x << "\n";
	f1.flush(); }
	mtx.unlock();
	f1.close();
};

void cpp_tracef(const char* fmt, ...) {// форматированная запись трейса C++.

	va_list args; char buf[2048];
	va_start(args, fmt);
	vsnprintf(buf, sizeof(buf), fmt, args);
	va_end(args);
	buf[sizeof(buf) - 1] = '\0';
	cpp_trace(buf);
};
