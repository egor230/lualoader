
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
#include "extensions\KeyCheck.h"
#include "extensions\ScriptCommands.h"
#include "eScriptCommands.h"
#include "CCivilianPed.h"
#include "CMessages.h"
#include "ePedModel.h"
#include "ePedType.h"
#include "CModelInfo.h"

#include "CStreaming.h"
#include "CTheScripts.h"
#include "eWeaponType.h"
#include "eWeaponModel.h"
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
#include "tHandlingData.h"
#include "CMatrix.h"
#include "CVector.h"
#include "CCopPed.h"
#include "CHeli.h"


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

void getkeyenvent(); // считывания символов клавиатуры.
wchar_t* getwchat(const char* c); // перевод в строку.

int funs(lua_State* L);// список функций.
void writelog(const char x[]);// запись ошибок в файл.
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
int ped_in_targetting(lua_State* L); // пед на прицеле.
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
			car->m_nLockStatus = status;
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
				if (car != v && DistanceBetweenPoints(car->GetPosition(), p->GetPosition()) < radius && car->m_fHealth > 50) {
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
				if (ped != p && DistanceBetweenPoints(ped->GetPosition(), p->GetPosition()) < radius && ped->m_fHealth > 50) {
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
				if (car != v && DistanceBetweenPoints(car->GetPosition(), p->GetPosition()) < radius && car->m_fHealth > 50) {

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
				if (DistanceBetweenPoints(obj->GetPosition(), p->GetPosition()) < radius) {
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
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.
			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			ped->SetObjective(OBJECTIVE_LEAVE_CAR);
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

			float x = lua_tonumber(L, 2); float y = lua_tonumber(L, 3);
			float z = lua_tonumber(L, 4); CVector pos = { x, y, z };// вектор для координат.
			ped->SetObjective(OBJECTIVE_SPRINT_TO_AREA, pos);// пед делает спринт к точке.
		}
		else { throw "bad argument in function ped_run_to_point"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int ped_walk_to_point(lua_State* L) {// пед идет пешком.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {// указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			float x = lua_tonumber(L, 2);	float y = lua_tonumber(L, 3);
			float z = lua_tonumber(L, 4);
			CVector pos = { x, y, z };
			ped->SetObjective(OBJECTIVE_GOTO_AREA_ON_FOOT, pos);// пед идет пешком.
		}
		else { throw "bad argument in function ped_walk_to_point"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
};

int ped_run_to_point(lua_State* L) {// пед бежит к точке пешком.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) &&
			LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)) {// указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			float x = lua_tonumber(L, 2);	float y = lua_tonumber(L, 3);
			float z = lua_tonumber(L, 4);
			CVector pos = { x, y, z };
			ped->SetObjective(OBJECTIVE_RUN_TO_AREA, pos);// пед бежит к точке пешком.
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
			angle = obj->GetHeading();
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
			angle = ped->GetHeading();// получить угол педа
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
			CVector pos = ped->m_placement.pos;
			pos += ped->m_placement.right * x;
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

			float y = lua_tonumber(L, 2);
			CVector pos = ped->m_placement.pos;
			pos += ped->m_placement.up * y;   lua_pushnumber(L, pos.x);
			lua_pushnumber(L, pos.y);   lua_pushnumber(L, pos.z);
			return 3;
		}
		else { throw "bad argument in function getpedcoordinates_on_y"; }
	}
	catch (const char* x) {	writelog(x);}
};

int getcarcoordinates_on_x(lua_State* L) {// Получить мировую координату по x для авто.
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на авто.

			const void* p = lua_topointer(L, 1);
			CVehicle* car = findcarinpool(p);//  получить указатель на авто.

			float x = lua_tonumber(L, 2);
			CVector pos = car->m_placement.pos;
			pos += car->m_placement.right * x;
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

			float y = lua_tonumber(L, 2);
			CVector pos = car->m_placement.pos;
			pos += car->m_placement.up * y; lua_pushnumber(L, pos.x);
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
			CVector pos = p1->m_placement.pos + p1->m_placement.right * x + p1->m_placement.up * y;
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

			Command<COMMAND_MARK_CAR_AS_NO_LONGER_NEEDED>(CPools::GetVehicleRef(car));// удалить авто.
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
			((int(__cdecl*)(int))0x4D1E90)(wanted);//	Command<COMMAND_SET_MAX_WANTED_LEVEL>(wanted);
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
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
			ped->ClearInvestigateEvent();// пед уходит, опустить педа.
			Command<COMMAND_MARK_CHAR_AS_NO_LONGER_NEEDED>(CPools::GetPedRef(ped));// удалить педа.
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
			CVector pos = obj->m_placement.pos;
			pos += obj->m_placement.right * x;
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
			CVector pos = obj->m_placement.pos;
			pos += obj->m_placement.up * y; lua_pushnumber(L, pos.x);
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
	return 1;
};

int setflagmission(lua_State* L) {// уcтановить флага миссии.
	unsigned int& OnAMissionFlag = *(unsigned int*)0x978748;
	try {
		if (LUA_TBOOLEAN == lua_type(L, 1)) {// значение число.
			bool flag = lua_toboolean(L, 1);
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
			map<int, lua_State*>::iterator it;

			for (auto it = markeron.begin(); it != markeron.end(); ++it) {
				if (L == it->second && blip== it->first) {
					markeron.erase(blip);
				}
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
	Command<COMMAND_LOAD_ALL_MODELS_NOW>(false);
	Command<COMMAND_REQUEST_MODEL>(model);
	Command<COMMAND_LOAD_ALL_MODELS_NOW>(false);
	while (!Command<COMMAND_HAS_MODEL_LOADED>(model)) {
		this_thread::sleep_for(chrono::milliseconds(1));// задержка
		Command<COMMAND_REQUEST_MODEL>(model);
	}
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
			//cam = (CCamera*)0x7E4688;
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
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TLIGHTUSERDATA == lua_type(L, 2)) {// указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);//  получить указатель на авто.
			p = lua_topointer(L, 2);
			CPed* ped2 = findpedinpool(p);//  получить указатель на авто.
			ped->SetObjective(OBJECTIVE_SET_LEADER, ped2);
			return 0;
		}
		else { throw "bad argument in function follow_the_leader"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int arrested(lua_State* L) { // пед арестован?
	if (CWorld::Players[CWorld::PlayerInFocus].m_nPlayerState == PLAYERSTATE_HASBEENARRESTED) {
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
			float angle = car->GetHeading();// получить угол авто
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
			cam = (CCamera*)0x7E4688;
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
			cam = (CCamera*)0x7E4688;
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
			cam = (CCamera*)0x7E4688;
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
	cam = (CCamera*)0x7E4688;
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

			CWeapon weapon = ped->m_aWeapons[ped->m_nActiveWeaponSlot];
			int ammo = weapon.m_nTotalAmmo; // кол-во патроны текущего оружие.
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

			CWeapon weapon = ped->m_aWeapons[ped->m_nActiveWeaponSlot];// получить оружие текущего слота. 
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
			CWeapon weapon = ped->m_aWeapons[ped->m_nActiveWeaponSlot];// получить оружие из текущего слота.
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
			CPed* ped = NULL; load_model_before_avalible(model);	
			//CWorld::Players[CWorld::PlayerInFocus].m_nMoney += type;// дать денег 
			Command<COMMAND_CREATE_CHAR>(type, model, pos.x, pos.y, pos.z, &ped);
			Command<COMMAND_MARK_MODEL_AS_NO_LONGER_NEEDED>(model);
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
			std::thread(expectations, model, std::ref(car)).detach();

			car->m_nLockStatus = 1;
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

			CWeapon weapon = ped->m_aWeapons[ped->m_nActiveWeaponSlot];
			eWeaponType type = weapon.m_nType;
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
			if (ped->m_bInVehicle && ped->m_pVehicle != NULL) {// в авто пед?
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

			CWeapon weapon = ped->m_aWeapons[ped->m_nActiveWeaponSlot];
			eWeaponType type = weapon.m_nType;
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

			CWeapon weapon = ped->m_aWeapons[ped->m_nActiveWeaponSlot];
			int type = weapon.m_nType;
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

			CWeapon weapon = ped->m_aWeapons[ped->m_nActiveWeaponSlot];
			eWeaponType type = weapon.m_nType;
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
			CWeapon weapon = ped->m_aWeapons[ped->m_nActiveWeaponSlot];

			eWeaponType type = weapon.m_nType;
			CWeaponInfo* w = CWeaponInfo::GetWeaponInfo(type);
			w->m_nDamage = damage;
			return 0;
		}
		else { throw "bad argument in function set_damage_current_weapon"; }
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
	float x = patch::GetFloat(0x7E46B8);
	float y = patch::GetFloat(0x7E46BC);
	float z = patch::GetFloat(0x7E46C0);
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

				ped->m_nPedFlags.bMoneyHasBeenGivenByScript = false;
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

			CWeapon w = ped->m_aWeapons[ped->m_nActiveWeaponSlot];
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
			CWeapon weapon = player->m_aWeapons[player->m_nActiveWeaponSlot];
			CVector pos1 = { ped->GetPosition().x, ped->GetPosition().y, float(ped->GetPosition().z + 1.5) };
			//CWeapon weapon = ped->m_aWeapons[ped->m_nActiveWeaponSlot];
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
			CWeapon weapon = player->m_aWeapons[player->m_nActiveWeaponSlot];

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
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {// указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);// получить указатель на педа.
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
	auto j = std::experimental::filesystem::current_path();
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
	try {
		if (LUA_TLIGHTUSERDATA == lua_type(L, 1)) {// указатель на педа.

			const void* p = lua_topointer(L, 1);
			CPed* ped = findpedinpool(p);//  получить указатель на авто.
			Command<COMMAND_LEAVE_GROUP>(CPools::GetPedRef(ped));
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
			angle = car->GetHeading();// получить угол авто.
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
			pos = car->m_placement.pos + car->m_placement.right * width + car->m_placement.up * length;
			car_cord[0][0] = pos.x; car_cord[0][1] = pos.y;//правый верхний угол первого авто.

			pos = car->m_placement.pos + car->m_placement.right * (width * -1) + car->m_placement.up * length;
			car_cord[0][2] = pos.x; car_cord[0][3] = pos.y;//левый верхний угол первого авто.
			// 1 сторона
			//car[0][0] = x; 	car[0][1] = y;  car[0][2] = x_end; car[0][3] = y_end;

			car_cord[1][0] = pos.x; car_cord[1][1] = pos.y;

			pos = car->m_placement.pos + car->m_placement.right * (width * -1) + car->m_placement.up * (length * -1);
			car_cord[1][2] = pos.x; car_cord[1][3] = pos.y;	;//левый нижний угол первого авто.
			// 2 сторона. 	//car[1][0] = 3; car[1][1] = 2;  car[1][2] = 3; car[1][3] = 1;

			car_cord[2][0] = pos.x; car_cord[2][1] = pos.y;

			pos = car->m_placement.pos + car->m_placement.right * width + car->m_placement.up * (length * -1);
			car_cord[2][2] = pos.x; car_cord[3][3] = pos.y;//правый нижний угол первого авто.
			// 3 сторона //car[2][0] = 3; car[2][1] = 1;  car[2][2] = 1; car[3][3] = 1;

			car_cord[3][0] = pos.x; car_cord[3][1] = pos.y;

			pos = car->m_placement.pos + car->m_placement.right * width + car->m_placement.up * length;
			car_cord[3][2] = pos.x; car_cord[3][3] = pos.y;//правый верхний угол первого авто.	
			// 4 сторона.			
			//car[3][0] = 1; car[3][1] = 1;  car[3][2] = 1; car[3][3] = 2;//координанты вершин 1 прямоугольника.

			// второе авто.
			pos = car1->m_pHandlingData->m_vDimensions; 	length = pos.x; width = pos.y; height = pos.z;

			length = +0.98;	width = (width + 0.64);// / 2;
			
			pos = car1->m_placement.pos + car1->m_placement.right * width + car1->m_placement.up * length;
			car_cord1[0][0] = pos.x; car_cord1[0][1] = pos.y;//правый верхний угол второго авто.

			pos = car1->m_placement.pos + car1->m_placement.right * (width * -1) + car1->m_placement.up * length;
			car_cord1[0][2] = pos.x; car_cord1[0][3] = pos.y;//левый верхний угол второго авто.
			// 1 сторона
			//car[0][0] = 1; 	car[0][1] = 2;  car[0][2] = 3; car[0][3] = 2;

			car_cord1[1][0] = pos.x; car_cord1[1][1] = pos.y;

			pos = car1->m_placement.pos + car1->m_placement.right * (width * -1) + car1->m_placement.up * (length * -1);
			car_cord1[1][2] = pos.x; car_cord1[1][3] = pos.y; ;//левый нижний угол второго авто.
			// 2 сторона. 	//car[1][0] = 3; car[1][1] = 2;  car[1][2] = 3; car[1][3] = 1;

			car_cord1[2][0] = pos.x; car_cord1[2][1] = pos.y;

			pos = car1->m_placement.pos + car1->m_placement.right * width + car1->m_placement.up * (length * -1);
			car_cord1[2][2] = pos.x; car_cord1[3][3] = pos.y;//правый нижний угол второго авто.
			// 3 сторона //car[2][0] = 3; car[2][1] = 1;  car[2][2] = 1; car[3][3] = 1;

			car_cord1[3][0] = pos.x; car_cord1[3][1] = pos.y;

			pos = car1->m_placement.pos + car1->m_placement.right * width + car1->m_placement.up * length;
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
				if (car != v && DistanceBetweenPoints(car->GetPosition(), pos) < radius) {
					car->CanPedExitCar(true);
					if (CPed * p1 = car->m_pDriver) {
						if (p1 != NULL && p1 != p) {
							lua_pushboolean(L, true); lua_pushlightuserdata(L, p1);// отправить в стек и получить из стека можно
							return 2;
			}	}	}
			}
			for (auto ped : CPools::ms_pPedPool) {
				if (ped != p && DistanceBetweenPoints(ped->GetPosition(), pos) < radius) {
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
				if (car != v && DistanceBetweenPoints(car->GetPosition(), pos) < radius) {
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
				if (DistanceBetweenPoints(obj->GetPosition(), pos) < radius) {
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
					if (car != v && DistanceBetweenPoints(car->GetPosition(), pos) < radius) {
						car->CanPedExitCar(true);
						if (CPed * p1 = car->m_pDriver) {
							if (p1 != NULL && p1 != p) {
								lua_pushboolean(L, true); lua_pushlightuserdata(L, p1);// отправить в стек и получить из стека можно
								return 2;}	}
					}
				}
				for (auto ped : CPools::ms_pPedPool) {
					if (ped != p && DistanceBetweenPoints(ped->GetPosition(), pos) < radius) {
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
      			  if (car != v && DistanceBetweenPoints(car->GetPosition(), pos) < radius) {
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
					if (DistanceBetweenPoints(obj->GetPosition(), pos) < radius) {
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
				if (DistanceBetweenPoints(obj->GetPosition(), pos) < radius) {
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
				if (DistanceBetweenPoints(obj->GetPosition(), pos) < radius) {
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
				if (car != v && DistanceBetweenPoints(car->GetPosition(), pos) < radius) {
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
				if (ped != p && DistanceBetweenPoints(ped->GetPosition(), pos) < radius) {
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
				if (car != v && DistanceBetweenPoints(car->GetPosition(), pos) < radius) {
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
				if (ped != p && DistanceBetweenPoints(ped->GetPosition(), pos) < radius) {
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
				if (car != v && DistanceBetweenPoints(car->GetPosition(), pos) < radius) {
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
				if (car != v && DistanceBetweenPoints(car->GetPosition(), pos) < radius) {
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

			car->m_placement.SetRotate(x, y, z);

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
			car->m_placement.RotateX(x);
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
			car->m_placement.RotateY(y);
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
			car->m_placement.RotateZ(z);
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
			car->m_placement.SetRotateX(x);
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
			car->m_placement.SetRotateY(y);
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
			car->m_placement.SetRotateZ(z);
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
			const void* p = lua_topointer(L, 1);	float z = lua_tonumber(L, 2); //p1->m_placement.RotateZ(z);


			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			CVector entpos = car->m_placement.pos;

			(*(void(__thiscall*)(CMatrix*, float))0x4DF4F0)(&car->m_placement, 3.1415927f * z * 0.0055555557f);    //    CMatrix::SetRotateZ
					
			car->m_placement.pos = {
			car->m_placement.pos.x += entpos.x,
			car->m_placement.pos.y += entpos.y,
			car->m_placement.pos.z += entpos.z
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
			const void* p = lua_topointer(L, 1);	float z = lua_tonumber(L, 2); //p1->m_placement.RotateZ(z);
			
			CVehicle* car = findcarinpool(p);// получить указатель на авто.
			CVector entpos = car->m_placement.pos;

			(*(void(__thiscall*)(CMatrix*, float))0x4DF450)(&car->m_placement, 3.1415927f * z * 0.0055555557f);    //    CMatrix::SetRotateZ

			car->m_placement.pos = {
			car->m_placement.pos.x += entpos.x,
			car->m_placement.pos.y += entpos.y,
			car->m_placement.pos.z += entpos.z
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
			
			float z = lua_tonumber(L, 2); //p1->m_placement.RotateZ(z);
			CVector entpos = car->m_placement.pos;
		
			(*(void(__thiscall*)(CMatrix*, float))0x4DF3B0)(&car->m_placement, 3.1415927f * z * 0.0055555557f);    //    CMatrix::SetRotateZ

			//car->m_placement.pos = {
			car->m_placement.pos.x = entpos.x;
				car->m_placement.pos.y = entpos.y; 
				car->m_placement.pos.z = entpos.z; 
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


int create_rope_on_cords_with_swat(lua_State* L) { // создать веревку на координатах с бойцом спецназа.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 4)
			&& LUA_TNUMBER == lua_type(L, 5)) {// значение число.
			int model = lua_tointeger(L, 1);// модель педа.
			int type = lua_tointeger(L, 2);// тип педа.
			float x = lua_tonumber(L, 3); float y = lua_tonumber(L, 4);
			float z = lua_tonumber(L, 5); CVector pos = { x, y, z };
			Command<COMMAND_CREATE_SWAT_ROPE>(model, type, pos.x, pos.y, pos.z);
			return 0;
		}// int
		else { throw "bad argument in function create_rope_on_cords_with_swat"; }
	}
	catch (const char* x) { writelog(x); }// записать ошибку в файл.
	return 0;
};

int create_rope_on_cords(lua_State* L) { // создать веревку на координатах.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {// значение число.
			
			float x = lua_tonumber(L, 1); float y = lua_tonumber(L, 2);
			float z = lua_tonumber(L, 3); 
			CVector pos = { x, y, z };
			((char(__cdecl*)(float,float,float))0x632550)(x, y, z);
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
	car_martix[0] = car->m_placement.right.x;
	car_martix[1] = car->m_placement.right.y;
	car_martix[2] = car->m_placement.right.z;
	car_martix[3] = car->m_placement.up.x;
	car_martix[4] = car->m_placement.up.y;
	car_martix[5] = car->m_placement.up.z;
	car_martix[6] = car->m_placement.at.x;
	car_martix[7] = car->m_placement.at.y;
	car_martix[8] = car->m_placement.at.z;
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
	car_martix[0] = car->m_placement.right.x;
	car_martix[1] = car->m_placement.right.y;
	car_martix[2] = car->m_placement.right.z;
	car_martix[3] = car->m_placement.up.x;
	car_martix[4] = car->m_placement.up.y;
	car_martix[5] = car->m_placement.up.z;
	car_martix[6] = car->m_placement.at.x;
	car_martix[7] = car->m_placement.at.y;
	car_martix[8] = car->m_placement.at.z;
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