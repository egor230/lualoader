#define _CRT_SECURE_NO_WARNINGS
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

#include <algorithm>
#include<lua.hpp>
#include<LuaBridge.h>

#include "common.h"
#include "plugin.h"
#include "CWorld.h"
#include "extensions\KeyCheck.h"
#include "extensions\ScriptCommands.h"
#include "eScriptCommands.h"
#include "CMessages.h"
#include "ePedModel.h"
#include "ePedType.h"
#include "CModelInfo.h"

#include "CTheScripts.h"
#include "eWeaponType.h"
#include "eWeaponModel.h"
#include "CPickups.h"
#include "CClock.h"
#include "CUserDisplay.h"
using namespace plugin;
using namespace std;
using namespace luabridge;
namespace fs = std::experimental::filesystem;// ��� ������ lua lua ������.
static int iters = 0;
list<char*>listfile;//������ Lua ������.

list<lua_State*>luastate;// list ��� lua ���������.
static map<int, lua_State*>markeron;// map ��� ��������.
static map<int, lua_State*>spheres;// map ��� �������� ������.

static map<int, lua_State*>pickupsids;// map ��� �������.
static map<CPed*, lua_State*>mappeds;// map ��� �����.
static map<CVehicle*, lua_State*>mapcars;// map ��� ����.
static map<CObject*, lua_State*>mapobjs;// map ��� ��������.

static vector<const void*>vector_for_ped;//
vector<thread>t;//������ ��� lua �������.
bool reload = false;// ���� ������� ������ ������
static string cheatstr;// ������� ��������� � ����������.
string getkey(int key);

static bool printgame = false;// ���� ��� ������ �����.
string strprintgame;// ����� �� �����.

static int abc_x;
static int ord_y;
static float spacing;
static int font;
static float size_x;
static float size_y;
static int red;
static int blue;
static int green;
void funs(lua_State* L);// ������ �������.
void writelog(const char x[]);// ������ ������ � ����.
void dellod(); // ������� ��� ������.
int cleanstl();// ������� stl.

int wait(lua_State* L);// ��������
int findplayer(lua_State* L);// ����� ������.
int getpedhealth(lua_State* L);// �������� �������� ����.
int setpedhealth(lua_State* L);// ���������� �������� ����.

int getpedarmour(lua_State* L); // �������� ����� ����.
int setarmour(lua_State* L);// ���������� ����� ����.
int getcarhealth(lua_State* L); // �������� ���-�� �������� ����.
int setcarhealth(lua_State* L); // ���������� �������� ����.

int loadmodel(lua_State* L);// ��������� ������.
int createcar(lua_State* L);// ������� ���� �� �����������.
int availablemodel(lua_State* L);// �������� �� ������������� ������.
int releasemodel(lua_State* L);// ������� ������ �� ������.

int key(lua_State* L);// �������� �� ������� �������.
int lockstatus(lua_State* L);// �������� �� ������� �������.
int givemoney(lua_State* L);// ���� �����. 
int getpedcoordes(lua_State* L);// �������� ����������.

int printmessage(lua_State* L);// ������� ��������� �� �����.
int randomfindped(lua_State* L);// ����� ����������� ����.
int incar(lua_State* L);// ��� � ����?
int exitcar(lua_State* L);// ����� �� ����.

int createped(lua_State* L);// ������� ����.
int create_marker_actor(lua_State* L);// ������� ������ ��� ����.
int removemarker(lua_State* L);// ������� ������.
int ped_sprint_to_point(lua_State* L);//��� ������ ������ � �����.

int ped_walk_to_point(lua_State* L);// ��� ���� � �����.
int getpedangle(lua_State* L);// �������� ���� ������.
int load_requested_models(lua_State* L);// ��������� ������ ��� �������.
int giveweaponped(lua_State* L);// ���� ���� ������.

int ped_aim_at_ped(lua_State* L);// ��� �������� � ����.
int is_current_weapon_ped(lua_State* L);// ��������� ������� ������.
int worldcoord(lua_State* L); // ������� � ������� ����������.
int getcoordinates_on_abscissa(lua_State* L); // �������� ������� ���������� �� x.

int getcoordinates_on_ordinate(lua_State* L); // �������� ������� ���������� �� y.
int kill_ped_on_foot(lua_State* L);// ����� ���� ������.
int kill_char_any_means(lua_State* L);// ����� ���� ������ ����������.

int create_sphere(lua_State* L); //������� �����.
int remove_sphere(lua_State* L); // ������� �����.
int remove_ped(lua_State* L); // ������� ����.
int remove_car(lua_State* L); // ������� ����.

int car_in_water(lua_State* L); // �������� ���� � ����?.
int set_wanted(lua_State* L); // �c�������� ������� �������.
int clear_wanted(lua_State* L);// ������ ������� �������.
int kill_ped(lua_State* L); // ����� ����.

int setpedcoordes(lua_State* L); // ���������� ���������� ��� ����.
int ped_in_point_in_radius(lua_State* L); // ��������� ��������� ��� � ����������� � ��������.
int cardrive(lua_State* L); // ���� ���� � �����.

int setcarspeed(lua_State* L); // ���������� �������� ����.
int getflagmission(lua_State* L); // �������� ����� ������.
int setflagmission(lua_State* L); // �c�������� ����� ������.
int showtext(lua_State* L);// ����� ������� ������ �� �����.

int remove_blip(lua_State* L);// ������� ����� � �����.
int createblip(lua_State* L); // ������� ����� �� �����.
int play_sound(lua_State* L);// ��������� �������.
int isped(lua_State* L); // �������� ��� ���?.

int isvehicle(lua_State* L); // �������� ��� ���������?.
int opendoorcar(lua_State* L); // ������� ����� ����.
int randomfindcar(lua_State* L); //����� ��������� ����.
int create_money_pickup(lua_State* L); //������� ����� �����.

int getcarcoordes(lua_State* L); // �������� ���������� ����.
int getcarcoordinates_on_abscissa(lua_State* L);// �������� ������� ���������� �� x ��� ����.
int getcarcoordinates_on_ordinate(lua_State* L); // �������� ������� ���������� �� y ��� ����.
int car_in_point_in_radius(lua_State* L); // ��������� ��������� ���� � ����������� � ��������.

int setdrivingstyle(lua_State* L); // ���������� ����� ����  ����.
int findped(lua_State* L); // ����� ���� � ����.
int create_weapon_pickup(lua_State* L); //������� ����� ������.
int create_pickup(lua_State* L); //������� �����.

int picked_up(lua_State* L); // ����� ��������.
int remove_pickup(lua_State* L); // ������� �����.
int play_voice(lua_State* L); // ��������� �����(�������).
int fade(lua_State* L);//���������, ������������.

int draw_corona(lua_State* L); // ������� ������(�������� ��������).
int sound_coordinate(lua_State* L); // ��������� ���� � �����������
int show_text_styled(lua_State* L); // ������� ������� �����.
int setcarangle(lua_State* L); // ���������� ���� ����.

int createmarker(lua_State* L); // ������� ������ �� �����.
int setsizemarker(lua_State* L); //���������� ������ ����������� ������� �� �����.
int checkcheat(lua_State* L);//��� ��� ������?.
int destroy(lua_State* L);// �������� �������� �� ������ ��� ������������ �������. 

int my_yield(lua_State* L);//������������� ���������� �������.
int setcardrive(lua_State* L); //  ���������� �������� ��� ����.
int setcarpassenger(lua_State* L); // ���������� ��������� ��� ����.
int setcarfirstcolor(lua_State* L); // ���������� ������ ���� ����.

int setcarseconscolor(lua_State* L);// ���������� ������ ���� ����.
int set_traffic(lua_State* L); // ���������� ������ ����������.
int create_marker_car(lua_State* L); //������� ������ ��� ����.
int car_explode(lua_State* L); // �������� ����.

int is_car_stopped(lua_State* L); // ���� ������������. 
int create_explosion(lua_State* L); // ������� ����� �� �����������.
int set_status_engine(lua_State* L); // ���������� ��������� ��������� ����.
int player_defined(lua_State* L); // ����� ����������?

int setclock(lua_State* L); // ������ �����.
int arrested(lua_State* L); // ����� ���������?
int create_marker_pickup(lua_State* L);// ������� ������ ��� �������.
int createobj(lua_State* L); // ������� ������.

int remove_obj(lua_State* L); // ������� ������.
int setobj�coordes(lua_State* L); // ���������� ���������� ��� ������.
int getobjcoordes(lua_State* L); // �������� ���������� �������.
int create_marker_obj(lua_State* L); //������� ������ ��� ��������.

int isobject(lua_State* L); // �������� ��� ������?.
int setpedangle(lua_State* L); // ���������� ���� ����.
int setcaraction(lua_State* L);// ���������� ��������� ����.
int move_obj(lua_State* L); //������� ������.

int move_rotate(lua_State* L); // ������� ������.
int getobjangle(lua_State* L); // �������� ���� �������.
int findcar(lua_State* L); // ����� ����.
int setcartask(lua_State* L);// ���������� ������ ����.

int setcarcoordes(lua_State* L);// ���������� ���������� ����.
int is_car_stuck(lua_State* L);//03CE: car 12@ stuck ���� ������ ��������.
int is_car_upsidedown(lua_State* L); //01F4: car 12@ flipped ���� ������ �����������.
int is_car_upright(lua_State* L); // 020D: car 12@ flipped ���� ��������� ���������� ����������.

int find_road_for_car(lua_State* L); // ����� ������.
int setcarstrong(lua_State* L); // ������� ���� ����������.
int putincar(lua_State* L);// ����������� ���� � ����.
int game_font_print(lua_State* L); // ������� ��������� ������� �����.

int star_timer(lua_State* L); // �������� ������.
int stop_timer(lua_State* L); // ���������� ������.

int timer_donw(lua_State* L); //  ������ �� ����������.
int ped_attack_car(lua_State* L); // ��� ������� ����.
int ped_frozen(lua_State* L); // ���������� ��p���.
int hold_cellphone(lua_State* L); // ������� �������.

int car_lastweapondamage(lua_State* L); // ����� ������, ������� ������� ���� ����.
int car_currentgear(lua_State* L); // ������� �������� ����.
int getcar_model(lua_State* L); // �������� ������ ����.
int setcarsiren(lua_State* L); // ���������� ������ ��� ����.

int ped_car_as_driver(lua_State* L); // ��� ������� � ���� ��� ��������.
int ped_car_as_passenger(lua_State* L); // ��� ������� � ���� ��� ��������.

int newthread(lua_State* L);// ������ ������� � ����� ������.
int counts = 0;//������� ��������.

int& var_$3402 = *(int*)0x8247A8; // ����������� ���������� �������.
const float ACTION_TIME_STEP = 0.05f;
const unsigned int TIME_FOR_KEYPRESS = 500;
struct Doorse {
	static int componentByDoorId[6]; // ������� �������� eDoors � Id ����������
	static int m_nLastTimeWhenAnyActionWasEnabled; // ��������� ����� ������� �������
	enum eDoorEventType { DOOR_EVENT_OPEN, DOOR_EVENT_CLOSE };// ��� �������

	struct DoorEvent { // ����� �������
		bool m_active;	eDoorEventType m_type;	float m_openingState;
		DoorEvent() { m_active = false;	m_type = DOOR_EVENT_CLOSE; }
	};

	struct VehicleDoors {
		DoorEvent events[6]; // ������� ��� ���� 6 ������
		VehicleDoors(CVehicle*) {}
	};
};
struct DoorsExample {
	static int componentByDoorId[6]; // ������� �������� eDoors � Id ����������
	static int m_nLastTimeWhenAnyActionWasEnabled; // ��������� ����� ������� �������

	enum eDoorEventType { DOOR_EVENT_OPEN, DOOR_EVENT_CLOSE };// ��� �������

	struct DoorEvent {
		bool m_active;	eDoorEventType m_type;	float m_openingState;
		DoorEvent() { m_active = false;	m_type = DOOR_EVENT_CLOSE; }// ����� �������
	};

	struct VehicleDoors {
		DoorEvent events[6]; // ������� ��� ���� 6 ������
		VehicleDoors(CVehicle*) {}
	};

	static VehicleExtendedData<VehicleDoors> VehDoors; // ���� ����������

	static void EnableDoorEvent(CAutomobile* automobile, eDoors doorId) { // �������� ������� �����
		if (automobile->IsComponentPresent(componentByDoorId[doorId])) {
			CDamageManager* p;
			if (p->GetDoorStatus(doorId) != DAMSTATE_NOTPRESENT) {
				DoorEvent& event = VehDoors.Get(automobile).events[doorId];
				if (event.m_type == DOOR_EVENT_OPEN)
					event.m_type = DOOR_EVENT_CLOSE; // ���� ��������� ������� - ��������, �� ���������
				else
					event.m_type = DOOR_EVENT_OPEN; // ���� ��������� ������� �������� - �� ���������
				event.m_active = true; // �������� ���������
				m_nLastTimeWhenAnyActionWasEnabled = CTimer::m_snTimeInMilliseconds;
			}
		}
	};

	static void ProcessDoors(CVehicle* vehicle) { // ��������� ������� ��� ����������� ����
		if (vehicle->m_nVehicleClass == VEHICLE_AUTOMOBILE) {
			CAutomobile* automobile = reinterpret_cast<CAutomobile*>(vehicle);
			for (unsigned int i = 0; i < 6; i++) { // ������������ ��� �������
				eDoors doorId = static_cast<eDoors>(i);
				DoorEvent& event = VehDoors.Get(automobile).events[doorId];
				if (event.m_active) { // ���� ������� �������
					if (event.m_type == DOOR_EVENT_OPEN) {
						event.m_openingState += ACTION_TIME_STEP;
						if (event.m_openingState > 1.0f) { // ���� ��������� �������
							event.m_active = false; // ��������� ���������
							automobile->OpenDoor(componentByDoorId[doorId], doorId, 1.0f); // ��������� ���������
							event.m_openingState = 1.0f;
						}
						else
							automobile->OpenDoor(componentByDoorId[doorId], doorId, event.m_openingState);
					}
					else {
						event.m_openingState -= ACTION_TIME_STEP;
						if (event.m_openingState < 0.0f) { // ���� ��������� �������
							event.m_active = false; // ��������� ���������
							automobile->OpenDoor(componentByDoorId[doorId], doorId, 0.0f); // ��������� ���������
							event.m_openingState = 0.0f;
						}
						else
							automobile->OpenDoor(componentByDoorId[doorId], doorId, event.m_openingState);
					}
				}
			}
		}
	};
};
int DoorsExample::componentByDoorId[6] = { CAR_BONNET, CAR_BOOT, CAR_DOOR_LF, CAR_DOOR_RF, CAR_DOOR_LR, CAR_DOOR_RR };
int DoorsExample::m_nLastTimeWhenAnyActionWasEnabled = 0;
VehicleExtendedData<DoorsExample::VehicleDoors> DoorsExample::VehDoors;
void reversestack(lua_State* L) {
	int stacksize = lua_gettop(L);
	stacksize++;
	for (int i = 1; i < stacksize; i++) { lua_insert(L, i); }
};

int my_yield(lua_State* L) {
	return lua_yield(L, 0);//������������� ���������� �������.
};
lua_KFunction cont(lua_State* L);
int my_yield_with_res(lua_State* L, int res) {
	return lua_yieldk(L, 0, lua_yield(L, res), cont(L));/* int lua_yieldk(lua_State * L, int res, lua_KContext ctx, lua_KFunction k);
	���������������� ���������� �����������(�����).	����� ������� C �������� lua_yieldk, ����������
	����������� ���������������� ���� ���������� � �������� lua_resume, ������� �������� ������� ������ �����������.
	�������� res - ��� ����� �������� �� �����, ������� ����� �������� � �������� ����������� � lua_resume.
	����� ����������� ����� ���������� ����������, Lua ������� �������� ������� ����������� k ��� ����������� ����������
	���������������� C �������(�������� �4.7). */
};
int hookFunc(lua_State* L, lua_Debug* ar) {
	return my_yield_with_res(L, 0);// ���./
};

lua_KFunction cont(lua_State* L) {// ������� �����������.
	lua_sethook(L, (lua_Hook)hookFunc, LUA_MASKCOUNT, 0);// ��������� ���.
	return 0;
};
void showstack(lua_State* L) {
	int i = lua_gettop(L);/* �������� ���������� ��������� � �����.*/
	string path = "stack.txt";
	fstream f2; {f2.open("stack.txt", fstream::in | fstream::out | fstream::app);
	if (f2.is_open()) {// ���� ���� ���� �������.
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
		if (LUA_TUSERDATA == t) {
			f1 << "user ";
			f1 << i; f1 << "\n";
		}
		if (LUA_TTHREAD == t) {
			cout << "LUA_TTHREAD " << endl;
		}
		if (LUA_TNIL == t) {
			cout << "LUA_TNIL " << endl;
		}
	}
	f1.close(); }
};
void showstack1(lua_State* L) {
	int i = lua_gettop(L);/* �������� ���������� ��������� � �����.*/
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
		if (LUA_TUSERDATA == t) {
			f1 << "user ";
			f1 << i; f1 << "\n";
		}
		if (LUA_TTHREAD == t) {
			cout << "LUA_TTHREAD " << endl;
		}
		if (LUA_TNIL == t) {
			cout << "LUA_TNIL " << endl;
		}
	}
	f1.close(); }
};// ���-�� ����������.

void startscipt(string res, char* luafile, list<lua_State*>& luastate, int& counts) {
	lua_State* L = luaL_newstate();	luaL_openlibs(L);
	funs(L); // ������ �������.	
	int status = luaL_loadfile(L, luafile);// �������� ���� �� ������ � �����.
	try {
		if (status == 0) {// ���� ��� ������ � �����.
			lua_pushlightuserdata(L, L);  /* ���� � ������ ��������� �� L. */
			lua_pushstring(L, luafile);  /* ��������� ��� �������� lua ����� � ������.*/
			lua_settable(L, LUA_REGISTRYINDEX);  /* ���������� ����� � �������� ������� �������.  */
			string er0 = "loaded " + res;// ����� ����� �������� lua ����� �������� loaded.
			char* x = strdup(er0.c_str());// ������������� ������ � char*.
			writelog(x);// ������ ���������� �������� �� ������.
			lua_pcall(L, 0, 0, 0);// ������ �����.
			lua_State* L1 = lua_newthread(L);// ������� ����� �����.
			lua_getglobal(L, "main"); counts++;// ��������� ������� ���������� �������� �� 1.
			if (LUA_TFUNCTION == lua_type(L, -1)) {
				luastate.push_back(L);// �������� ��������� �� lua ��������� � ������.
				lua_resume(L, NULL, 0);	//�������� �����
				int args = lua_gettop(L);// �������� ��������� ��� ������� ������.
				lua_xmove(L, L1, args);
				args--;
				reversestack(L1); //������������� ���������� �����.
				while (LUA_OK != lua_status(L)) {// ���� �������� ����� �� ��������.
					this_thread::sleep_for(chrono::milliseconds(1)); // ��������.
					if (LUA_TFUNCTION == lua_type(L1, -1) && LUA_YIELD == lua_status(L)) {

						for (int i = 1; i <= args; i++) { lua_pushvalue(L1, i); }// ����������� ���������� ��� ������.
						lua_resume(L1, L, args);
					}
					if (LUA_YIELD == lua_status(L1)) {
						lua_sethook(L, (lua_Hook)hookFunc, LUA_MASKCOUNT, 9000); //����� ������� � �������� ������.
						lua_resume(L, L1, 0);// ����������� �������� �����.
					}
					if (LUA_OK == lua_status(L1)) {// ���� ������ ����� ��������.      
						lua_sethook(L, (lua_Hook)hookFunc, LUA_MASKCOUNT, 0);// ��������� ���.
						lua_resume(L, NULL, 0);
					}
				}
			}
			else {   /*lua_close(L);*/ }
		}
		else {
			string er1 = lua_tostring(L, -1);
			string er0 = "could not load " + er1;
			char* x = strdup(er1.c_str());  throw x;
		}
	}
	catch (const char* x) { writelog(x); }
};
void quit() { exit(0); }// ������� ����������� ������.
void search() {
	for (auto const& de : fs::recursive_directory_iterator{ fs::current_path() / "lualoader" }) { // ����� ��� ������
		if (de.path().extension() == ".lua" || de.path().extension() == ".LUA") {
			string res = de.path().string();// ������� ��� ����� � ������.
			char* luafile = strdup(res.c_str());// ������� lua ����.
			listfile.push_back(luafile);// �������� ������� lua ���� � list.
			t.push_back(move((std::thread(startscipt, res, luafile, std::ref(luastate), std::ref(counts)))));// �������� ����� � ������.
		}
	};
};
char q;
static	string faststr = "";
void getkeyenvent(bool& redload) {// ���������� �������� ����������.
	while (reload == true) {
		this_thread::sleep_for(chrono::milliseconds(1));
		for (q = 8; q <= 190; q++) {
			string faststr = getkey(q);// �������� ������ ������� �������.
			while (true) {
				this_thread::sleep_for(chrono::milliseconds(1));
				string f2 = getkey(q);// ���� ���������� �������.
				if (faststr != f2) {
					break;
				}
			};
			if (faststr != "") { cheatstr = cheatstr + faststr; }
			int size = cheatstr.size();// ���� ����� ������ ������ 10 ��������, ������� ������.
			if (size > 9) {
				cheatstr.clear();
			}
		}
	}
};
void second(bool& reload) {
	dellod(); // ������� ��� ������.
	search(); // ����� ��� lua �����.
	thread k(getkeyenvent, std::ref(reload)); k.detach();// ������� ���������� �� ����������.
	static unsigned int time = 0;// �������� ������.
	while (true) {
		this_thread::sleep_for(chrono::milliseconds(1));
		if (KeyPressed(VK_CONTROL)) {// ������������ �������    
			break;
		};
	};
	for (auto L : luastate) {
		if (LUA_TFUNCTION == lua_type(L, -1)) {
			lua_setglobal(L, "lualoader");// ���������� �������� ���������� � lua.
			lua_pushinteger(L, 110); destroy(L); luastate.remove(L);
		}
		else {destroy(L);  luastate.remove(L);
		}
	};
	if (!luastate.empty()) {
		for (auto L : luastate) {
			luastate.pop_front();
		};
	};
	cleanstl();// ������� ���� �������� ��� ������������ �������.
	if (CTimer::m_snTimeInMilliseconds - time > 500) {
		time = 0;// �������� ������
		CMessages::AddMessageJumpQ(L"Script reloaded", 2000, 3);
	};
	while (true) {
		this_thread::sleep_for(chrono::milliseconds(1));
		if (!KeyPressed(VK_CONTROL)) {
			break;
		}
	};
	this_thread::sleep_for(chrono::milliseconds(10));
	reload = false; // ����, ��� ����� ��������� ����� �����.
};

class Message {//��� ������
public: Message() {
	Events::gameProcessEvent += [] {//���������� ������� ����
		iters++;
		Events::vehicleRenderEvent += DoorsExample::ProcessDoors; // ��� ������������ �������, � ����� ��������� ��
		CPed* player = FindPlayerPed();// ����� ������.
		if (player != NULL) {// �������� ������ �����
			static unsigned int time = 0;// �������� ������

			if (reload == false && player != NULL) {// ����� ����� �� �������, ����� ����������
				if (CTimer::m_snTimeInMilliseconds - time > 500) {
					this_thread::sleep_for(chrono::milliseconds(160));// ��������
					time = 0;// �������� ������
				}
				thread th(second, std::ref(reload)); th.detach();// ����������� �����.       
				reload = true;// ����, ��� ��� ������� �����. 
			};
		}
		if (printgame == true){
		gamefont::Print({Format(strprintgame) }, abc_x,	 ord_y,	 spacing, font, size_x, size_y, CRGBA(red, blue, green));
		}
	};
}
} message;

void funs(lua_State* L) {// ������ �������.
	getGlobalNamespace(L)//������������ ���� LuaBridge ��� ����������� ������� � �������, ����� ������ ��������� Lua.
		.beginClass<CPed>("cped")// ��� ������ ��� � lua.
		.endClass()// ������� ����������� ������.  
		.beginClass<CVehicle>("CVehicle")// ��� ������ ���� � lua.
		.endClass()// ������� ����������� ������.
		.beginClass<CObject>("CObject")// ��� ������ ������� � lua.
		.endClass()// ������� ����������� ������.
		.addCFunction("findplayer", findplayer)// ���������� ��������� ������.

		.addCFunction("setpedhealth", setpedhealth)//  ���������� �������� ����.
		.addCFunction("setarmour", setarmour)//  ���������� ����� ����.
		.addCFunction("wait", wait)//  ��������.
		.addCFunction("getpedhealth", getpedhealth)// �������� ������� � lua � c++. �������� �������� ����.

		.addCFunction("getpedangle", getpedangle)// �������� ���� ����.
		.addCFunction("worldcoord", worldcoord)// ������� � ������� ����������.
		.addCFunction("getcoordinates_on_x", getcoordinates_on_abscissa)// �������� ������� ���������� �� x.
		.addCFunction("getcoordinates_on_y", getcoordinates_on_ordinate) // �������� ������� ���������� �� y.

		.addCFunction("setarmour", setarmour)//  �������� �����.
		.addCFunction("givemoney", givemoney)//  ���� ����� ������.
		.addCFunction("keypress", key)//  �������� �� ������� ������.
		.addCFunction("printmessage", printmessage)//  ����� ���������.

		.addCFunction("getpedcoordes", getpedcoordes)//  �������� ���������� ������.
		.addCFunction("randomfindped", randomfindped)//  �������� ���������� ����.
		.addCFunction("incar", incar)//  �������� ��� � ����?.
		.addCFunction("loadmodel", loadmodel)// ��������� ������.

		.addCFunction("availablemodel", availablemodel)// �������� �� ������������� ������.
		.addCFunction("releasemodel", releasemodel)// ������� ������ �� ������.
		.addCFunction("createcar", createcar)// ������� ���� �� �����������.
		.addCFunction("createped", createped) // ������� ���� �� �����������.

		.addCFunction("load_requested_models", load_requested_models)// ��������� ������ �� ��������� ��� �������.
		.addCFunction("giveweaponped", giveweaponped)// ���� ���� ������.
		.addCFunction("ped_sprint_to_point", ped_sprint_to_point)// ��� ������ ������ � �����.
		.addCFunction("ped_walk_to_point", ped_walk_to_point)//��� ���� � �����.

		.addCFunction("kill_ped_on_foot", kill_ped_on_foot)// ����� ���� ������.
		.addCFunction("kill_char_any_means", kill_char_any_means)// ����� ���� ������ ����������.
		.addCFunction("ped_aim_at_ped", ped_aim_at_ped)// ��� �������� � ����.
		.addCFunction("is_current_weapon_ped", is_current_weapon_ped)// ��������� ������� ������.

		.addCFunction("create_marker_actor", create_marker_actor)// ������� ������ ��� �����.
		.addCFunction("removemarker", removemarker)// ������� ������.
		.addCFunction("setpedcoordes", setpedcoordes) // ���������� ���������� ��� ����.
		.addCFunction("remove_car", remove_car) // ������� ����.

		.addCFunction("car_in_water", car_in_water) // �������� ���� � ����.
		.addCFunction("set_wanted", set_wanted) // �c�������� ������� �������.
		.addCFunction("ped_in_point_in_radius", ped_in_point_in_radius) // ��������� ��������� ��� � ����������� � ��������.
		.addCFunction("create_sphere", &create_sphere) //������� �����.

		.addCFunction("clear_wanted", clear_wanted)// ������ ������� �������.
		.addCFunction("getcarhealth", getcarhealth) // �������� ���-�� �������� ����.
		.addCFunction("setcarhealth", setcarhealth) // ���������� �������� ����.
		.addCFunction("remove_sphere", remove_sphere) // ������� �����.

		.addCFunction("remove_ped", remove_ped) // ������� ����.
		.addCFunction("kill_ped", kill_ped) // ����� ����.
		.addCFunction("getflagmission", getflagmission) // �������� ����� ������.
		.addCFunction("setflagmission", setflagmission) // �c�������� ����� ������.

		.addCFunction("showtext", showtext)// ����� ������� ������ �� �����.
		.addCFunction("remove_blip", remove_blip)// ������� ����� � �����.
		.addCFunction("createblip", createblip) // ������� ����� �����.
		.addCFunction("play_sound", play_sound)// ��������� �������.

		.addCFunction("isped", isped)// �������� ��� ���?
		.addCFunction("isvehicle", isvehicle) // �������� ��� ���������?.
		.addCFunction("cardrive", &cardrive) // ���� ���� � �����.
		.addCFunction("setcarspeed", setcarspeed) // ���������� �������� ����.

		.addCFunction("opendoorcar", opendoorcar) // ������� ����� ����.
		.addCFunction("randomfindcar", randomfindcar) //����� ��������� ����.
		.addCFunction("getcarcoordes", getcarcoordes) // �������� ���������� ����.
		.addCFunction("create_money_pickup", create_money_pickup) //������� ����� �����.

		.addCFunction("getcarcoordinates_on_x", getcarcoordinates_on_abscissa)// �������� ������� ���������� �� x ��� ����.
		.addCFunction("getcarcoordinates_on_y", getcarcoordinates_on_ordinate)// �������� ������� ���������� �� y ��� ����.
		.addCFunction("car_in_point_in_radius", car_in_point_in_radius) // ��������� ��������� ���� � ����������� � ��������.
		.addCFunction("setdrivingstyle", setdrivingstyle)// ���������� ����� ���� ����.

		.addCFunction("findped", findped)// ����� ���� � ����.
		.addCFunction("create_weapon_pickup", create_weapon_pickup) //������� ����� ������.
		.addCFunction("create_pickup", create_pickup) //������� �����.
		.addCFunction("remove_pickup", remove_pickup) // ������� �����.

		.addCFunction("picked_up", picked_up)// ����� ��������.
		.addCFunction("play_voice", play_voice) // ��������� �����.
		.addCFunction("fade", fade) //���������, ������������.
		.addCFunction("draw_corona", draw_corona) // ������� ������(��������).

		.addCFunction("sound_coordinate", sound_coordinate) // ��������� ���� � �����������
		.addCFunction("show_text_styled", show_text_styled) // ������� ������� �����.
		.addCFunction("setcarangle", setcarangle)// ���������� ���� ����.
		.addCFunction("createmarker", createmarker) // ������� ������ �� �����.

		.addCFunction("setsizemarker", setsizemarker)//���������� ������ �������.
		.addCFunction("cheat", checkcheat) //��� ��� ������.
		.addCFunction("destroy", destroy) // �������� �������� �� ������ ��� ������������ �������. 
		.addCFunction("yield", my_yield) //������������� ���������� �������.

		.addCFunction("setcardrive", setcardrive) // ���������� �������� ��� ����.
		.addCFunction("setcarpassenger", setcarpassenger) // ���������� ��������� ��� ����.
		.addCFunction("setcarfirstcolor", setcarfirstcolor) // ���������� ������ ���� ����.
		.addCFunction("setcarseconscolor", setcarseconscolor)// ���������� ������ ���� ����.

		.addCFunction("set_traffic", set_traffic) // ���������� ������ ����������.
		.addCFunction("create_marker_car", create_marker_car)//������� ������ ��� ����.
		.addCFunction("car_explode", car_explode) // �������� ����.
		.addCFunction("is_car_stopped", is_car_stopped) // ���� ������������. 

		.addCFunction("create_explosion", create_explosion) // ������� ����� �� �����������.
		.addCFunction("set_status_engine", set_status_engine) // ���������� ��������� ��������� ����.
		.addCFunction("player_defined", player_defined) // ����� ����������.
		.addCFunction("setclock", setclock) //  ������ �����.

		.addCFunction("arrested", arrested) // ����� ���������?
		.addCFunction("lockstatus", lockstatus)// ������ ����� ����.
		.addCFunction("create_marker_pickup", create_marker_pickup)// ������� ������ ��� �������.
		.addCFunction("create_obj", &createobj) // ������� ������.

		.addCFunction("remove_obj", remove_obj) // ������� ������.
		.addCFunction("setobj�coordes", setobj�coordes) // ���������� ���������� ��� ������.
		.addCFunction("getobjcoordes", getobjcoordes) // �������� ���������� �������.
		.addCFunction("create_marker_obj", create_marker_obj) //������� ������ ��� ��������.

		.addCFunction("isobject", isobject) // �������� ��� ������?.
		.addCFunction("setpedangle", setpedangle) // ���������� ���� ����.
		.addCFunction("setcaraction", setcaraction)// ���������� ��������� ����.
		.addCFunction("move_obj", move_obj) //������� ������.

		.addCFunction("move_rotate", move_rotate) //������� ������.
		.addCFunction("getobjangle", getobjangle) // �������� ���� �������.
		.addCFunction("findcar", findcar)//����� ����.
		.addCFunction("setcartask", setcartask) // ���������� ������ ����.

		.addCFunction("setcarcoordes", setcarcoordes)// ���������� ���������� ����.
		.addCFunction("is_car_stuck", is_car_stuck)//03CE: car 12@ stuck ���� ������ ��������.
		.addCFunction("is_car_upsidedown", is_car_upsidedown)//01F4: car 12@ flipped ���� ������ �����������.
		.addCFunction("is_car_upright", is_car_upright) // 020D: car 12@ flipped ���� ��������� ���������� ����������.

		.addCFunction("find_road_for_car", find_road_for_car) // ����� ������.
		.addCFunction("setcarstrong", setcarstrong) // ������� ���� ����������.
		.addCFunction("putincar", putincar)// ����������� ���� � ����.
		.addCFunction("print_front", game_font_print) // ������� ��������� ������� �����.

		.addCFunction("star_timer", star_timer) // �������� ������.
		.addCFunction("stop_timer", stop_timer) // ���������� ������.
		.addCFunction("timer_donw", timer_donw) //  ������ �� ����������.
		.addCFunction("ped_attack_car", ped_attack_car) // ��� ������� ����.

		.addCFunction("ped_frozen", ped_frozen)  // ���������� ��p���.
		.addCFunction("hold_cellphone", hold_cellphone) // ������� �������.
		.addCFunction("car_lastweapondamage", car_lastweapondamage)// ����� ������, ������� ������� ���� ����.
		.addCFunction("car_currentgear", car_currentgear) // ������� �������� ����.

		.addCFunction("getcar_model", getcar_model) // �������� ������ ����.
		.addCFunction("setcarsiren", setcarsiren) // ���������� ������ ��� ����.
		.addCFunction("ped_car_as_driver", ped_car_as_driver) // ��� ������� � ���� ��� ��������.
		.addCFunction("ped_car_as_passenger", ped_car_as_passenger) // ��� ������� � ���� ��� ��������.

			
		.addCFunction("newthread", newthread)// ������ ������� � ����� ������.
		.addCFunction("exitcar", exitcar);// �������� ������� � lua � c++. ����� �� ����.
};

void writelog(const char x[]) {// ������ ������ � ����.
	string path = "lualoader\\log.txt";
	fstream f1; {f1.open(path, fstream::in | fstream::out | fstream::app);
	f1 << x; time_t rawtime; struct tm* timeinfo;
	char buffer[80]; time(&rawtime); timeinfo = localtime(&rawtime);
	strftime(buffer, sizeof(buffer), " %I:%M:%S %d-%m-%Y", timeinfo);
	string er2(buffer); f1 << er2 << "\n"; }
	f1.close();
};

wchar_t* getwchat(const char* c) {// ������� � ������.
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize]; mbstowcs(wc, c, cSize);
	return wc;// ������� ������.
};
int findplayer(lua_State* L) {// �������� ��������� �� ������.
	CPed* player = FindPlayerPed();// ����� �����.
	Stack<CPed*>::push(L, player);// ��������� � ���� ��������� �� ������.
	return 1;
};

int cardrive(lua_State* L) {// ���� ���� � �����.
	try {
		if (LUA_TUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)
			&& LUA_TNUMBER == lua_type(L, 4)) {// �������� �����.
			CVehicle* vehicle = Stack<CVehicle*>::get(L, 1);// ������ ����.
			float x = Stack<float>::get(L, 2); float y = Stack<float>::get(L, 3);
			float z = Stack<float>::get(L, 4); CVector pos = { x, y, z };
			Command<COMMAND_CAR_GOTO_COORDINATES>(vehicle, pos.x, pos.y, pos.z);// ���� ���� �� ����������.
			return 0;
		}// int

		else { throw "bad argument in function cardrive"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int wait(lua_State* L) {
	try {
		if (LUA_TNUMBER == lua_type(L, -1)) {// �������� �����.
			int time = lua_tointeger(L, -1); // ����� ��������.
			this_thread::sleep_for(chrono::milliseconds(time));
			return 0;
		}// int
		if (LUA_TSTRING == lua_type(L, -1) || LUA_TBOOLEAN == lua_type(L, -1)) {
			throw "bad argument in function wait";
		}
		if (LUA_TBOOLEAN == lua_type(L, -1)) {
			throw "bad argument in function wait";
		}
		else { this_thread::sleep_for(chrono::milliseconds(1)); }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int setpedhealth(lua_State* L) {// ���������� �������� ����.
	try {
		if (LUA_TUSERDATA == lua_type(L, -2)) {// ��������� �� ������. 
			if (LUA_TNUMBER == lua_type(L, -1)) {
				CPed* player = (CPed*)Userdata::get<CPed>(L, 1, false);// �������� ��������� �� ������.
				float health = Stack<float>::get(L, 2);// ���� �����.
				health += 0.99f; player->m_fHealth = health; return 0;
			}// ���������� �������� ������.
			else { throw "bad argument in function setpedhealth option health"; }
		}
		else { throw "bad argument in function setpedhealth option of the player"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int setarmour(lua_State* L) {// ���������� ����� ����.
	try {
		if (LUA_TUSERDATA == lua_type(L, -2)) {// ��������� �� ������.
			if (LUA_TNUMBER == lua_type(L, -1)) {
				CPed* ped = (CPed*)Userdata::get<CPed>(L, 1, false);// �������� ��������� �� ������.
				float armour = Stack<float>::get(L, 2);
				armour += 0.10f; ped->m_fArmour = armour; return 0;
			}// ���������� ����� ������.
			else { throw "bad argument in function setarmour option health"; }
		}
		else { throw "bad argument in function setarmour option of the player"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int getpedarmour(lua_State* L) {
	try {
		if (LUA_TUSERDATA == lua_type(L, -1)) {// ��������� �� ������.
			CPed* ped = (CPed*)Userdata::get<CPed>(L, 1, false);// �������� ��������� �� ������.
			float armour = ped->m_fArmour;  return 1;
		}// �������� ����� ������.
		else { throw "bad argument in function getpedarmour option of the player"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int getpedhealth(lua_State* L) {
	try {
		if (LUA_TUSERDATA == lua_type(L, -1)) {// ��������� �� ������.
			CPed* b = (CPed*)Userdata::get<CPed>(L, 1, false);// �������� ��������� �� ������.
			int health = b->m_fHealth; // �������� ���-�� �������� ������.
			Stack<int>::push(L, health);// ��������� � ����.  
			return 1;
		}
		else { throw "bad argument in function getpedhealth option of the player"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int getcarhealth(lua_State* L) { // �������� ���-�� �������� ����.
	try {
		if (LUA_TUSERDATA == lua_type(L, -1)) {// ��������� �� ����.
			CVehicle* car = (CVehicle*)Userdata::get<CVehicle>(L, 1, false);// �������� ��������� �� ����.
			int health = car->m_fHealth; // �������� ���-�� �������� ����.
			Stack<int>::push(L, health);// ��������� � ����.  
			return 1;
		}
		else { throw "bad argument in function getpedhealth option of the vehicle"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int opendoorcar(lua_State* L) { // ������� ����� ����.
	try {
		if (LUA_TUSERDATA == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {// ��������� �� ����.
			CVehicle* b = (CVehicle*)Userdata::get<CVehicle>(L, 1, false);// �������� ��������� �� ����.
			CAutomobile* automobile = reinterpret_cast<CAutomobile*>(b); // ����� ��, ���������� �����. �.�. �� ����� ����� damageManager, ��� ����� ���������, ��� ��������� - ��� ���������� (CAutomobile)

			int door = Stack<int>::get(L, 2);
			switch (door) {
			case 0: {DoorsExample::EnableDoorEvent(automobile, BONNET); // 0 �����
				break;	 }
			case 1: {DoorsExample::EnableDoorEvent(automobile, BOOT); // 1 ��������
				break;	 }
			case 2: { DoorsExample::EnableDoorEvent(automobile, DOOR_FRONT_LEFT); // 2 ����� �������� �����
				break;	 }
			case 3: {DoorsExample::EnableDoorEvent(automobile, DOOR_FRONT_RIGHT); // 3 ������ �������� �����
				break;	 }
			case 4: {DoorsExample::EnableDoorEvent(automobile, DOOR_REAR_LEFT); // 4 ����� ������ �����
				break;	 }
			case 5: {DoorsExample::EnableDoorEvent(automobile, DOOR_REAR_RIGHT); // 5 ������ ������ �����
				break; }
			default: {}
			}
			return 0;
		}
		else { throw "bad argument in function opendoorcar option of the vehicle"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int setcarhealth(lua_State* L) {// ���������� �������� ����.
	try {
		if (LUA_TUSERDATA == lua_type(L, -2)) {// ��������� �� ����. 
			if (LUA_TNUMBER == lua_type(L, -1)) {// �������� ����.
				CVehicle* car = (CVehicle*)Userdata::get<CVehicle>(L, 1, false);// �������� ��������� �� ����.
				float health = Stack<float>::get(L, 2);// ���� �����.
				health += 0.99f; car->m_fHealth = health; return 0;
			}// ���������� �������� ����.
			else { throw "bad argument in function setcarhealth option health"; }
		}
		else { throw "bad argument in function setcarhealth option of the vehicle"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int setcarangle(lua_State* L) {// ���������� ���� ����.
	try {
		if (LUA_TUSERDATA == lua_type(L, -2)) {// ��������� �� ����. 
			if (LUA_TNUMBER == lua_type(L, -1)) {
				CVehicle* v = (CVehicle*)Userdata::get<CVehicle>(L, 1, false);// �������� ��������� �� ����.
				float angle = Stack<float>::get(L, 2);// ���� �����.

				Command<COMMAND_SET_CAR_HEADING>(CPools::GetVehicleRef(v), angle);
				return 0;
			}
			else { throw "bad argument in function setcarangle option health"; }
		}
		else { throw "bad argument in function setcarangle option of the vehicle"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int setdrivingstyle(lua_State* L) {// ���������� ����� ���� ����.
	try {
		if (LUA_TUSERDATA == lua_type(L, -2)) {// ��������� �� ����. 
			if (LUA_TNUMBER == lua_type(L, -1)) {
				CVehicle* car = (CVehicle*)Userdata::get<CVehicle>(L, 1, false);// �������� ��������� �� ����.
				int style = Stack<int>::get(L, 2);// ���� �����.
				switch (style) {
				case 0: {car->m_autoPilot.m_nDrivingStyle = DRIVINGSTYLE_STOP_FOR_CARS;
					break;	 }//	��������� ����� �������� ��� �����������; 
				case 1: {	car->m_autoPilot.m_nDrivingStyle = DRIVINGSTYLE_SLOW_DOWN_FOR_CARS;
					break;	 }// ����� �������� ��������� ��� �����������;,
				case 2: {car->m_autoPilot.m_nDrivingStyle = DRIVINGSTYLE_AVOID_CARS;
					break;	 }// ����� �������� ��������� �����������;,
				case 3: {car->m_autoPilot.m_nDrivingStyle = DRIVINGSTYLE_PLOUGH_THROUGH;
					break; }//	����� �������� ���� �����;,
				case 4: {	car->m_autoPilot.m_nDrivingStyle = DRIVINGSTYLE_STOP_FOR_CARS_IGNORE_LIGHTS;
					break; }//	����� �������� ���� ��� ����������� ������������ ����;
				default: {}
				}
				return 0;
			}// ���������� ����� ���� ����.
			else { throw "bad argument in function setdrivingstyle option style"; }
		}
		else { throw "bad argument in function setdrivingstyle option of the vehicle"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int setcaraction(lua_State* L) {// ���������� ��������� ����.
	try {
		if (LUA_TUSERDATA == lua_type(L, -3)) {// ��������� �� ����. 
			if (LUA_TNUMBER == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {
				CVehicle* car = (CVehicle*)Userdata::get<CVehicle>(L, 1, false);// �������� ��������� �� ����.
				int style = Stack<int>::get(L, 2);// ���� �����.
				unsigned int t = Stack<int>::get(L, 3);// ���� �����.
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
						if (style == 6) {//�����.
							Command<COMMAND_SET_CAR_TEMP_ACTION>(CPools::GetVehicleRef(vehicle), TEMPACT_TURNLEFT, time);
							return 0;
						}
						if (style == 7) {// ������.
							Command<COMMAND_SET_CAR_TEMP_ACTION>(CPools::GetVehicleRef(vehicle), TEMPACT_TURNRIGHT, time);
							return 0;
						}
						if (style == 8) {// ������.
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
			}// ���������� ����� ���� ����.
			else { throw "bad argument in function setcaraction"; }
		}
		else { throw "bad argument in function setcaraction option of the vehicle"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int setcarspeed(lua_State* L) {// ���������� �������� ����.
	try {
		if (LUA_TUSERDATA == lua_type(L, -2)) {// ��������� �� ����. 
			if (LUA_TNUMBER == lua_type(L, -1)) {
				CVehicle* mycar = (CVehicle*)Userdata::get<CVehicle>(L, 1, false);// �������� ��������� �� ����.
				float speed = Stack<float>::get(L, 2);// ���� �����.
				Command<COMMAND_SET_CAR_CRUISE_SPEED>(CPools::GetVehicleRef(mycar), speed);
				return 0;
			}
			else { throw "bad argument in function setcarspeed option speed"; }
		}
		else { throw "bad argument in function setcarspeed option of the vehicle"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int setcartask(lua_State* L) {// ���������� ������ ����.
	try {
		if (LUA_TUSERDATA == lua_type(L, -2)) {// ��������� �� ����. 
			if (LUA_TNUMBER == lua_type(L, -1)) {
				CVehicle* car = (CVehicle*)Userdata::get<CVehicle>(L, 1, false);// �������� ��������� �� ����.
				int task = Stack<int>::get(L, 2);// ���� �����.
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
					car->m_autoPilot.m_nCarMission = MISSION_RAMPLAYER_CLOSE;//����� �� �������
					return 0;
				}
				if (task == 4) {
					car->m_autoPilot.m_nCarMission = MISSION_BLOCKPLAYER_FARAWAY;
					return 0;
				}
				if (task == 5) {
					car->m_autoPilot.m_nCarMission = MISSION_BLOCKPLAYER_CLOSE;// ��������� � ������. 
					return 0;
				}
				if (task == 6) {//�����.
					car->m_autoPilot.m_nCarMission = MISSION_BLOCKPLAYER_HANDBRAKESTOP;
					return 0;
				}
				if (task == 7) {// ������.
					car->m_autoPilot.m_nCarMission = MISSION_WAITFORDELETION;
					return 0;
				}
				if (task == 8) {// ������.
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
	catch (const char* x) { writelog(x); }
	return 0;
};
int key(lua_State* L) {// �������� ������ �� �������?
	try {
		if (LUA_TNUMBER == lua_type(L, -1)) {// �������� �����.
			double key = lua_tonumber(L, -1);
			int x2 = (int)key;
			if (key == x2) {
				if (GetAsyncKeyState(key) == -32767) {
					Stack<bool>::push(L, true);// ������� ������.
					return 1;
				}
				else {
					Stack<bool>::push(L, false);
					return 1;
				}
			}
			else { throw "bad argument in function key"; }
		}
		else { throw "bad argument in function key"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int lockstatus(lua_State* L) {// ������ ����� ����.
	try {
		if (LUA_TUSERDATA == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {// ��������� �� ����. 
			CVehicle* car = (CVehicle*)Userdata::get<CVehicle>(L, 1, false);// �������� ��������� �� ����.
			int status = Stack<int>::get(L, 2);// ���� �����.
			car->m_nLockStatus = status;
			return 0;
		}
		else { throw "bad argument in function lockstatus"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int givemoney(lua_State* L) {
	try {
		if (LUA_TNUMBER == lua_type(L, -1)) {//���-�� �����.
			int money = lua_tonumber(L, -1);
			CWorld::Players[CWorld::PlayerInFocus].m_nMoney += money;// ���� �����  
		}
		else { throw "bad argument in function givemoney"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int getpedcoordes(lua_State* L) {// �������� ���������� ������.
	try {
		if (LUA_TUSERDATA == lua_type(L, -1)) {// ��������� �� ������.
			CPed* player = (CPed*)Userdata::get<CPed>(L, 1, false);// �������� ��������� �� ������.
			Stack<double>::push(L, player->GetPosition().x);// ��������� � ����.
			Stack<double>::push(L, player->GetPosition().y);// ��������� � ����.
			Stack<double>::push(L, player->GetPosition().z);// ��������� � ����.
			return 3;
		}// �������� ���������� ������.

		else { throw "bad argument in function getpedcoordes option of the player"; }
	}
	catch (const char* x) { writelog(x); }
};
int getcarcoordes(lua_State* L) {// �������� ���������� ����.
	try {
		if (LUA_TUSERDATA == lua_type(L, -1)) {// ��������� �� ����.

			CVehicle* car = (CVehicle*)Userdata::get<CVehicle>(L, 1, false);// �������� ��������� �� ����.
			Stack<double>::push(L, car->GetPosition().x);// ��������� � ����.
			Stack<double>::push(L, car->GetPosition().y);// ��������� � ����.
			Stack<double>::push(L, car->GetPosition().z);// ��������� � ����.
			return 3;
		}// �������� ���������� ����.

		else { throw "bad argument in function getcarcoordes"; }
	}
	catch (const char* x) { writelog(x); }
};
int printmessage(lua_State* L) {// ��������� ����� � � ����� ������ �� �����.
	try {
		if (LUA_TSTRING == lua_type(L, -3) && LUA_TNUMBER == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {//������.
			const char* c = lua_tostring(L, -3);// ������.
			int time = lua_tointeger(L, -2);// ����� ������ ������.
			int style = lua_tointeger(L, -1);// ����� ������.
			wchar_t* str = getwchat(c);
			CMessages::AddMessageJumpQ(str, time, style);// ������� ��������� �� �����.
			return 0;
		}
		else { throw "bad argument in function printmessage"; }
	}
	catch (const char* x) { writelog(x); }// �������� ������ � ����.
	return 0;
};
int randomfindped(lua_State* L) {// ����� ���� � �������.
	try {
		if (LUA_TUSERDATA == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {
			CPed* p = (CPed*)Userdata::get<CPed>(L, 1, false);// �������� ��������� �� ������.
			double radius = lua_tonumber(L, -1);
			CVehicle* v = NULL;

			if (p->m_bInVehicle && p->m_pVehicle != NULL) {// � ���� �����?
				CVehicle* v = p->m_pVehicle;
			}// �������� ��������� �� ����� ���� � ������� ����� �����.
			for (auto car : CPools::ms_pVehiclePool) {
				if (car != v && DistanceBetweenPoints(car->GetPosition(), p->GetPosition()) < radius && car->m_fHealth > 50) {
					car->CanPedExitCar(true);
					if (CPed * p1 = car->m_pDriver) {
						if (p1 != NULL && p1 != p) {
							Stack<bool>::push(L, true); Stack<CPed*>::push(L, p1);// ��������� � ���� � �������� �� ����� �����
							return 2;
						}
					}
				}
			}
			for (auto ped : CPools::ms_pPedPool) {
				if (ped != p && DistanceBetweenPoints(ped->GetPosition(), p->GetPosition()) < radius && ped->m_fHealth > 50) {
					Stack<bool>::push(L, true); Stack<CPed*>::push(L, ped);// ��������� � ���� � �������� �� ����� �����
					return 2;
				}
			}//    
			CPed* p2 = nullptr; Stack<bool>::push(L, false);
			Stack<CPed*>::push(L, p2);// ��������� � ���� � �������� �� ����� �����
			return 2;
		}
		else { throw "bad argument in function randomfindped"; }
	}
	catch (const char* x) { writelog(x); }// �������� ������ � ����.
};
int randomfindcar(lua_State* L) {//����� ��������� ���� � �������.
	try {
		if (LUA_TUSERDATA == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {
			CPed* p = (CPed*)Userdata::get<CPed>(L, 1, false);// �������� ��������� �� ������.
			double radius = Stack<int>::get(L, 2);// ������.
			CVehicle* v = NULL;

			if (p->m_bInVehicle && p->m_pVehicle != NULL) {// � ���� �����?
				CVehicle* v = p->m_pVehicle;
			}// �������� ��������� �� ����� ���� � ������� ����� �����.
			for (auto car : CPools::ms_pVehiclePool) {
				if (car != v && DistanceBetweenPoints(car->GetPosition(), p->GetPosition()) < radius && car->m_fHealth > 50) {

					Stack<bool>::push(L, true); Stack<CVehicle*>::push(L, car);// ��������� � ���� � �������� �� ����� �����
					return 2;
				}
			}
		}
		else { throw "bad argument in function randomfindcar"; }
	}
	catch (const char* x) {
		writelog(x);// �������� ������ � ����.
	}
};
int findcar(lua_State* L) {//����� ����.
	try {
		if (LUA_TUSERDATA == lua_type(L, -1)) {
			CVehicle* v = (CVehicle*)Userdata::get<CVehicle>(L, 1, false);// �������� ��������� �� ������.
			for (auto car : CPools::ms_pVehiclePool) {
				if (car == v) {
					Stack<CVehicle*>::push(L, car);// ��������� � ���� � �������� �� ����� �����
					return 1;
				}
			}
		}
		else { throw "bad argument in function findcar"; }
	}
	catch (const char* x) { writelog(x); }// �������� ������ � ����.
};
int findped(lua_State* L) {
	try {
		lua_newtable(L);
		luaL_newmetatable(L, "mt");
		for (auto ped : CPools::ms_pPedPool) {
			Stack<CPed*>::push(L, ped);
			Stack<CPed*>::push(L, ped);// ��������� � ���� � �������� �� ����� �����
			lua_rawset(L, -3);
			luaL_setmetatable(L, "mt");
		}
		return 1;
	}

	catch (const char* x) { writelog(x); }// �������� ������ � ����.
};

int incar(lua_State* L) {// ����� � ����?
	try {
		if (LUA_TUSERDATA == lua_type(L, -1)) {// ��������� �� ������.
			CPed* player = (CPed*)Userdata::get<CPed>(L, 1, false);// �������� ��������� �� ������.
			if (player->m_bInVehicle && player->m_pVehicle != NULL) {// � ���� �����?
				CVehicle* v = player->m_pVehicle;
				Stack<bool>::push(L, true);
				Stack<CVehicle*>::push(L, v);// ��������� � ���� true � ��������� �� ����.
				return 2;
			}
			else {
				CVehicle* v = NULL;//���� ��� �� � ���� ������� null;
				Stack<bool>::push(L, false);
				Stack<CVehicle*>::push(L, v);// ��������� � ���� � �������� �� ����� �����
				return 2;// �������� ��������� �� ����� ���� � ������� ����� ���.
			}
		}
		else { throw "bad argument in function incar"; }
	}
	catch (const char* x) { writelog(x); }
};

int exitcar(lua_State* L) {// ����� ������� �� ������.
	try {
		if (LUA_TUSERDATA == lua_type(L, -1)) {// ��������� �� ������.
			CPed* p = (CPed*)Userdata::get<CPed>(L, 1, false);// �������� ��������� �� ������.
			p->SetObjective(OBJECTIVE_LEAVE_CAR);
		} // ����� �� ����.
		else { throw "bad argument in function exitcar"; }
	}
	catch (const char* x) { writelog(x); }
};
int loadmodel(lua_State* L) {//�������� �������.
	try {
		if (LUA_TNUMBER == lua_type(L, -1)) {// �������� �����(������).
			int model = lua_tointeger(L, -1);
			Command<COMMAND_REQUEST_MODEL>(model);
			return 0;
		}// int

		else { throw "bad argument in function loadmodel"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int releasemodel(lua_State* L) {// ������� ������ �� ������.
	try {
		if (LUA_TNUMBER == lua_type(L, -1)) {// �������� �����.
			int model = lua_tointeger(L, -1);
			Command<COMMAND_MARK_MODEL_AS_NO_LONGER_NEEDED>(model);
			return 0;
		}// int
		else { throw "bad argument in function releasemodel"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int availablemodel(lua_State* L) {// �������� �� ������������� ������.
	try {
		if (LUA_TNUMBER == lua_type(L, -1)) {// �������� �����.
			int model = lua_tointeger(L, -1);
			bool ava = Command<COMMAND_HAS_MODEL_LOADED>(model);
			Stack<bool>::push(L, ava);
			return 1;
		}// int

		else { throw "bad argument in function availablemodel"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int createcar(lua_State* L) {// ������� ����.
	try {
		if (LUA_TNUMBER == lua_type(L, -4) && LUA_TNUMBER == lua_type(L, -3) && LUA_TNUMBER == lua_type(L, -2)
			&& LUA_TNUMBER == lua_type(L, -1)) {// �������� �����.
			int model = Stack<int>::get(L, -4);// ������ ����.
			float x = Stack<float>::get(L, -3); float y = Stack<float>::get(L, -2);
			float z = Stack<float>::get(L, -1); CVector pos = { x, y, z };
			CVehicle* vehicle = nullptr;
			Command<COMMAND_CREATE_CAR>(model, pos.x, pos.y, pos.z, &vehicle);
			mapcars.emplace(vehicle, L);// �������� � map ��� ����.
			int vehicle1 = (int)& vehicle;
			lua_pushinteger(L, vehicle1);  /* ��������� ����� ������ ���������� � ���� */
			lua_pushstring(L, "cvehicle");  /* ��������� �������� � ���� */
			lua_settable(L, LUA_REGISTRYINDEX);  /* ���������� ����� � �������� ������� �������.  */
			Stack<CVehicle*>::push(L, vehicle);// ��������� � ���� ��������� �� ����.
			return 1;
		}// int

		else { throw "bad argument in function createcar"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int createobj(lua_State* L) {// ������� ������.
	try {
		if (LUA_TNUMBER == lua_type(L, -4) && LUA_TNUMBER == lua_type(L, -3) && LUA_TNUMBER == lua_type(L, -2)
			&& LUA_TNUMBER == lua_type(L, -1)) {// �������� �����.
			int model = Stack<int>::get(L, -4);// ������ ����.
			float x = Stack<float>::get(L, -3); float y = Stack<float>::get(L, -2);
			float z = Stack<float>::get(L, -1); CVector pos = { x, y, z };
			CObject* obj = nullptr;
			Command<COMMAND_CREATE_OBJECT>(model, pos.x, pos.y, pos.z, &obj);
			int obj1 = (int)& obj;
			mapobjs.emplace(obj, L);// �������� � map ��� ����.

			lua_pushinteger(L, obj1);  /* ��������� ����� ������ ���������� � ���� */
			lua_pushstring(L, "cobject");  /* ��������� �������� � ���� */
			lua_settable(L, LUA_REGISTRYINDEX);  /* ���������� ����� � �������� ������� �������.  */

			Stack<CObject*>::push(L, obj);// ��������� � ���� ��������� �� ������.
			return 1;
		}// int

		else { throw "bad argument in function createobj"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int create_marker_actor(lua_State* L) {//������� ������ ��� �����.
	int marker;
	try {
		if (LUA_TUSERDATA == lua_type(L, -1)) {// ��������� �� ������.
			CPed* b = (CPed*)Userdata::get<CPed>(L, 1, false);// �������� ��������� �� ������.
			Command<COMMAND_ADD_BLIP_FOR_CHAR>(CPools::GetPedRef(b), &marker);
			markeron.emplace(marker, L);// �������� � map ��� ��������.
			Stack<int>::push(L, marker);// ��������� � ����.  
			return 1;
		}
		else { throw "bad argument in function create_marker_actor"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int create_marker_car(lua_State* L) {//������� ������ ��� ����.
	int marker;
	try {
		if (LUA_TUSERDATA == lua_type(L, 1)) {// ��������� �� ����.
			CVehicle* car = (CVehicle*)Userdata::get<CVehicle>(L, 1, false);// �������� ��������� �� ����.
			Command<COMMAND_ADD_BLIP_FOR_CAR>(CPools::GetVehicleRef(car), &marker);

			markeron.emplace(marker, L);// �������� � map ��� ��������.
			Stack<int>::push(L, marker);// ��������� � ����.  
			return 1;
		}
		else { throw "bad argument in function create_marker_car"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int removemarker(lua_State* L) {// ������� ������.
	try {
		if (LUA_TNUMBER == lua_type(L, -1)) {// �������� �����.
			int marker = Stack<int>::get(L, -1);// �������� id �������.
			Command<COMMAND_REMOVE_BLIP>(marker);
			return 0;
		}
		else { throw "bad argument in function removemarker option of the player"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int ped_sprint_to_point(lua_State* L) {// ��� ������ ������ � �����.
	try {
		if (LUA_TUSERDATA == lua_type(L, -4) && LUA_TNUMBER == lua_type(L, -3) &&
			LUA_TNUMBER == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {// ��������� �� ������.
			CPed* p = (CPed*)Userdata::get<CPed>(L, 1, false);// �������� ��������� �� ������.
			float x = Stack<float>::get(L, -3); float y = Stack<float>::get(L, -2);
			float z = Stack<float>::get(L, -1); CVector pos = { x, y, z };// ������ ��� ���������.
			p->SetObjective(OBJECTIVE_SPRINT_TO_AREA, pos);// ��� ������ ������ � �����.
		}
		else { throw "bad argument in function ped_run_to_point"; }
	}
	catch (const char* x) {
		writelog(x);
	}
};
int ped_walk_to_point(lua_State* L) {// ��� ���� ������.
	try {
		if (LUA_TUSERDATA == lua_type(L, -4) && LUA_TNUMBER == lua_type(L, -3) &&
			LUA_TNUMBER == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {// ��������� �� ������.
			CPed* p = (CPed*)Userdata::get<CPed>(L, 1, false);// �������� ��������� �� ������.
			float x = Stack<float>::get(L, -3);
			float y = Stack<float>::get(L, -2);
			float z = Stack<float>::get(L, -1);
			CVector pos = { x, y, z };
			p->SetObjective(OBJECTIVE_GOTO_AREA_ON_FOOT, pos);// ��� ���� ������.
		}
		else { throw "bad argument in function ped_walk_to_point"; }
	}
	catch (const char* x) {
		writelog(x);
	}
};
int getobjangle(lua_State* L) {// �������� ���� �������.
	try {
		double angle;
		if (LUA_TUSERDATA == lua_type(L, -1)) {// ��������� �� ������.
			CObject* obj = (CObject*)Userdata::get<CObject>(L, 1, false);
			Command<COMMAND_GET_OBJECT_HEADING>(CPools::GetObjectRef(obj), angle);
			Stack<double>::push(L, angle);// ��������� � ����.
			return 1;
		}// �������� ���� �������.

		else { throw "bad argument in function getobjangle"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int getpedangle(lua_State* L) {// �������� ���� ����
	try {
		if (LUA_TUSERDATA == lua_type(L, -1)) {// ��������� �� ������.
			CPed* ped = (CPed*)Userdata::get<CPed>(L, 1, false);// �������� ��������� �� ������.
			float angle;// ���������� ������� ���� ����.
			CPed* player = FindPlayerPed();// ����� ������.
			if (ped == player) {
				Command<COMMAND_GET_PLAYER_HEADING>(CWorld::PlayerInFocus, &angle);//  �������� ���� ������.
				Stack<int>::push(L, angle);// ��������� � ����.  
				return 1;
			}
			else {
				Command<COMMAND_GET_CHAR_HEADING>(CPools::GetPedRef(ped), &angle);//  �������� ���� ����.
				Stack<int>::push(L, angle);// ��������� � ����.  
				return 1;
			}
		}
		else { throw "bad argument in function getpedangle option of the player"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int setpedangle(lua_State* L) {// ���������� ���� ����.
	try {
		if (LUA_TUSERDATA == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {// ��������� �� ����. 
			CPed* ped = (CPed*)Userdata::get<CPed>(L, 1, false);// �������� ��������� �� ������.
			float angle = Stack<float>::get(L, 2);// ���� �����.
			CPed* player = FindPlayerPed();// ����� ������.
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
	catch (const char* x) { writelog(x); }
	return 0;
};
int getcoordinates_on_abscissa(lua_State* L) {// �������� ������� ���������� �� x.
	try {
		if (LUA_TUSERDATA == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {// ��������� �� ������.
			CPed* p = (CPed*)Userdata::get<CPed>(L, 1, false);// �������� ��������� �� ������.
			float x = Stack<float>::get(L, -1);
			CVector pos = p->m_placement.pos;
			pos += p->m_placement.right * x;
			Stack<float>::push(L, pos.x); Stack<float>::push(L, pos.y); Stack<float>::push(L, pos.z);
			return 3;
		}
		else { throw "bad argument in function getcoordinates_on_abscissa"; }
	}
	catch (const char* x) {
		writelog(x);
	}
};
int getcoordinates_on_ordinate(lua_State* L) {// // �������� ������� ���������� �� y.
	try {
		if (LUA_TUSERDATA == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {// ��������� �� ������.
			CPed* p = (CPed*)Userdata::get<CPed>(L, 1, false);// �������� ��������� �� ������.
			float y = Stack<float>::get(L, -1);
			CVector pos = p->m_placement.pos;
			pos += p->m_placement.up * y;   Stack<float>::push(L, pos.x);
			Stack<float>::push(L, pos.y);   Stack<float>::push(L, pos.z);
			return 3;
		}
		else { throw "bad argument in function getcoordinates_on_ordinate"; }
	}
	catch (const char* x) {
		writelog(x);
	}
};
int getcarcoordinates_on_abscissa(lua_State* L) {// �������� ������� ���������� �� x ��� ����.
	try {
		if (LUA_TUSERDATA == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {// ��������� �� ������.
			CVehicle* p = (CVehicle*)Userdata::get<CVehicle>(L, 1, false);// �������� ��������� �� ������.
			float x = Stack<float>::get(L, -1);
			CVector pos = p->m_placement.pos;
			pos += p->m_placement.right * x;
			Stack<float>::push(L, pos.x); Stack<float>::push(L, pos.y); Stack<float>::push(L, pos.z);
			return 3;
		}
		else { throw "bad argument in function getcarcoordinates_on_abscissa"; }
	}
	catch (const char* x) {
		writelog(x);
	}
};
int getcarcoordinates_on_ordinate(lua_State* L) {// // �������� ������� ���������� �� y ��� ����.
	try {
		if (LUA_TUSERDATA == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {// ��������� �� ������.
			CVehicle* p = (CVehicle*)Userdata::get<CVehicle>(L, 1, false);// �������� ��������� �� ������.
			float y = Stack<float>::get(L, -1);
			CVector pos = p->m_placement.pos;
			pos += p->m_placement.up * y;   Stack<float>::push(L, pos.x);
			Stack<float>::push(L, pos.y);   Stack<float>::push(L, pos.z);
			return 3;
		}
		else { throw "bad argument in function getcarcoordinates_on_ordinate"; }
	}
	catch (const char* x) { writelog(x); }
};

int worldcoord(lua_State* L) {// ������� � ������� ����������.
	try {
		if (LUA_TUSERDATA == lua_type(L, -3) && LUA_TNUMBER == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {// ��������� �� ������.
			CPed* p = (CPed*)Userdata::get<CPed>(L, 1, false);// �������� ��������� �� ������.
			float x = Stack<float>::get(L, -2); float y = Stack<float>::get(L, -1);
			CVector pos = p->m_placement.pos + p->m_placement.right * x + p->m_placement.up * y;
			Stack<float>::push(L, pos.x);   Stack<float>::push(L, pos.y);
			return 2;
		}
		else { throw "bad argument in function worldcoord"; }
	}
	catch (const char* x) { writelog(x); }
};
int load_requested_models(lua_State* L) {// �������� ������ � �� �������.
	Command<COMMAND_LOAD_ALL_MODELS_NOW>(false);
	return 0;
};
int giveweaponped(lua_State* L) {
	try {
		if (LUA_TUSERDATA == lua_type(L, -4)) {// ��������� �� ������.
			if (LUA_TNUMBER == lua_type(L, -1) && (LUA_TNUMBER == lua_type(L, -2))) {
				unsigned int model = Stack<unsigned int>::get(L, -3);// ������ ������.
				unsigned int WEAPONTYPE = Stack<unsigned int>::get(L, -2);// ��� �����.
				int ammo = Stack<int>::get(L, -1);// ����� ��������.
				CPed* v = (CPed*)Userdata::get<CPed>(L, 1, false);// �������� ��������� �� ������.
				CPed* player = FindPlayerPed();// ����� ������
				if (v == player) { Command<COMMAND_GIVE_WEAPON_TO_PLAYER>(CWorld::PlayerInFocus, WEAPONTYPE, ammo); }
				else {
					Command<COMMAND_GIVE_WEAPON_TO_CHAR>(CPools::GetPedRef(v), WEAPONTYPE, ammo);// ���� ������ ����.
				}
			}
			else { throw "bad argument in function giveweaponped option weapons"; }
		}
		else { throw "bad argument in function giveweaponped option of the player"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int kill_ped_on_foot(lua_State* L) {
	static int numberped;// ������� ����� ��� ���������� �����.
	static CPed* pedfoe;// ������� ��������� �����.
	try {
		if (LUA_TUSERDATA == lua_type(L, -1)) {// ��������� �� ����.
			if (numberped != 1) {
				numberped = 1;//��������� ����� ����, ����� �������� � 2.
				CPed* ped = (CPed*)Userdata::get<CPed>(L, 1, false);// �������� ��������� �� ����.
				pedfoe = ped;
				return 0;
			};
			if (numberped == 1) {
				CPed* ped2 = (CPed*)Userdata::get<CPed>(L, 1, false);// �������� ��������� �� ����.
				pedfoe->SetObjective(OBJECTIVE_KILL_CHAR_ON_FOOT, ped2);
				numberped = NULL;
				return 0;
			}
		}
		else { throw "bad argument in function kill_ped_on_foot option of the player"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int kill_char_any_means(lua_State* L) {
	static int numberped;;// ������� ����� ��� ���������� �����.
	static CPed* pedfoe;// ������� ��������� �����.
	try {
		if (LUA_TUSERDATA == lua_type(L, -1)) {// ��������� �� ����.
			if (numberped != 1) {
				numberped = 1;//��������� ����� ����, ����� �������� � 2.
				CPed* ped = (CPed*)Userdata::get<CPed>(L, 1, false);// �������� ��������� �� ����.
				pedfoe = ped;
				return 0;
			};
			if (numberped == 1) {
				CPed* ped2 = (CPed*)Userdata::get<CPed>(L, 1, false);// �������� ��������� �� ����.
				pedfoe->SetObjective(OBJECTIVE_KILL_CHAR_ANY_MEANS, ped2);
				numberped = NULL;
				return 0;
			}
		}
		else { throw "bad argument in function kill_char_any_means option of the ped"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int ped_aim_at_ped(lua_State* L) {//��� �������� � ����.
	try {
		if (LUA_TUSERDATA == lua_type(L, -1) && LUA_TUSERDATA == lua_type(L, -2)) {// ��������� �� ����.
			CPed* ped = (CPed*)Userdata::get<CPed>(L, 1, false);// �������� ��������� �� ����.
			CPed* ped2 = (CPed*)Userdata::get<CPed>(L, 2, false);// �������� ��������� �� ����.
			ped->SetObjective(OBJECTIVE_AIM_GUN_AT, ped2);// ��������� ���� �������� � ������� ����.
			return 0;
		}
		else { throw "bad argument in function ped_aim_at_ped option of the ped"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int is_current_weapon_ped(lua_State* L) {
	try {
		if (LUA_TUSERDATA == lua_type(L, -2) && (LUA_TNUMBER == lua_type(L, -1))) {// ��������� �� ������.
			unsigned int weapon_type = Stack<unsigned int>::get(L, -1);// ��� ������.
			CPed* ped = (CPed*)Userdata::get<CPed>(L, 1, false);// �������� ��������� �� ������.
			CPed* player = FindPlayerPed();// ����� ������
			if (ped != player) {
				bool charweapontype = Command<COMMAND_IS_CURRENT_CHAR_WEAPON>(CPools::GetPedRef(ped), weapon_type);
				Stack<bool>::push(L, charweapontype);
				return 1;
			}
			else {
				bool playerweapontype = Command<COMMAND_IS_CURRENT_PLAYER_WEAPON>(CWorld::PlayerInFocus, weapon_type);
				Stack<bool>::push(L, playerweapontype);// �������� ������� �������� ���������� � ������� ������ ������.
				return 1;
			}
		}
		else { throw "bad argument in function is_current_weapon_ped"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int create_sphere(lua_State* L) {//������� �����.
	try {
		if (LUA_TNUMBER == lua_type(L, -4) && LUA_TNUMBER == lua_type(L, -3) &&
			LUA_TNUMBER == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {
			float x = Stack<float>::get(L, 1);  float y = Stack<float>::get(L, 2);
			float z = Stack<float>::get(L, 3);  float radius = Stack<float>::get(L, 4);
			int sphere;// ����������, ������� ������� id �����.  
			CVector pos = { x, y, z };
			Command<COMMAND_ADD_SPHERE>(pos.x, pos.y, pos.z, radius, &sphere); //�������, �������, ������� �����  
			Command<COMMAND_REMOVE_SPHERE>(sphere);// �����, ����� ������ ����.
			Command<COMMAND_ADD_SPHERE>(pos.x, pos.y, pos.z, radius, &sphere);
			lua_settop(L, 0);// �������� ����.
			spheres.emplace(sphere, L);
			Stack<int>::push(L, sphere);// ��������� id ����� � ����.  
			return 1;
		}
		else { throw "bad argument in function create_sphere "; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int create_money_pickup(lua_State* L) {//������� ����� �����.
	try {
		if (LUA_TNUMBER == lua_type(L, -4) && LUA_TNUMBER == lua_type(L, -3) &&
			LUA_TNUMBER == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {
			int money = Stack<int>::get(L, 1); float x = Stack<float>::get(L, 2);
			float y = Stack<float>::get(L, 3); float z = Stack<float>::get(L, 4);
			int idpickup;// ����������, ������� ������� id �����.  
			CVector pos = { x, y, z };
			Command<COMMAND_CREATE_MONEY_PICKUP>(pos.x, pos.y, pos.z, money, &idpickup);
			pickupsids.emplace(idpickup, L);
			Stack<int>::push(L, idpickup);// ��������� id ������ � ����.  
			return 1;
		}
		else { throw "bad argument in function create_money_pickup"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int create_weapon_pickup(lua_State* L) {//������� ����� ������.
	try {
		if (LUA_TNUMBER == lua_type(L, -6) && LUA_TNUMBER == lua_type(L, -5)
			&& LUA_TNUMBER == lua_type(L, -4) && LUA_TNUMBER == lua_type(L, -3) &&
			LUA_TNUMBER == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {
			int model = Stack<int>::get(L, 1); int type = Stack<int>::get(L, 2);
			int ammo = Stack<int>::get(L, 3); float x = Stack<float>::get(L, 4);
			float y = Stack<float>::get(L, 5); float z = Stack<float>::get(L, 6);
			int idpickup;// ����������, ������� ������� id ������.  
			CVector pos = { x, y, z };
			Command<COMMAND_CREATE_PICKUP_WITH_AMMO>(model, type, ammo, pos.x, pos.y, pos.z, &idpickup);
			pickupsids.emplace(idpickup, L);
			Stack<int>::push(L, idpickup);// ��������� id ������ � ����.  
			return 1;
		}
		else { throw "bad argument in function create_weapon_pickup"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int create_pickup(lua_State* L) {//������� �����.
	try {
		if (LUA_TNUMBER == lua_type(L, -5) && LUA_TNUMBER == lua_type(L, -4) && LUA_TNUMBER == lua_type(L, -3) &&
			LUA_TNUMBER == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {
			int model = Stack<int>::get(L, 1); int type = Stack<int>::get(L, 2);
			float x = Stack<float>::get(L, 3);	float y = Stack<float>::get(L, 4);
			float z = Stack<float>::get(L, 5);
			int idpickup;// ����������, ������� ������� id ������.  
			CVector pos = { x, y, z };
			Command<COMMAND_CREATE_PICKUP>(model, type, pos.x, pos.y, pos.z, &idpickup);
			pickupsids.emplace(idpickup, L);
			Stack<int>::push(L, idpickup);// ��������� id ������ � ����.  
			return 1;
		}
		else { throw "bad argument in function create_weapon_pickup"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int remove_sphere(lua_State* L) {// ������� �����.
	try {
		if (LUA_TNUMBER == lua_type(L, -1)) {// �������� �����.
			int sphere = Stack<int>::get(L, 1);
			Command<COMMAND_REMOVE_SPHERE>(sphere);// ������� �����.
			return 0;
		}
		else { throw "bad argument in function remove_sphere"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int picked_up(lua_State* L) {// ����� ��������.
	try {
		if (LUA_TNUMBER == lua_type(L, -1)) {// �������� �����.
			int pickup = Stack<int>::get(L, 1);
			bool checkpickup = Command<COMMAND_HAS_PICKUP_BEEN_COLLECTED>(pickup);
			Stack<bool>::push(L, checkpickup);
			return 1;
		}
		else { throw "bad argument in function picked_up"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int remove_pickup(lua_State* L) {// ������� �����.
	try {
		if (LUA_TNUMBER == lua_type(L, -1)) {// �������� id ������.
			int pickup = Stack<int>::get(L, 1);
			Command<COMMAND_REMOVE_PICKUP>(pickup);// ������� �����.
			return 0;
		}
		else { throw "bad argument in function remove_pickup"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int remove_car(lua_State* L) {// ������� ����.
	try {
		if (LUA_TUSERDATA == lua_type(L, -1)) {// �������� �����.
			CVehicle* car = (CVehicle*)Userdata::get<CVehicle>(L, 1, false);
			Command<COMMAND_MARK_CAR_AS_NO_LONGER_NEEDED>(CPools::GetVehicleRef(car));// ������� ����.
			return 0;
		}
		else { throw "bad argument in function remove_car"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int remove_obj(lua_State* L) {// ������� ������.
	try {
		if (LUA_TUSERDATA == lua_type(L, -1)) {// �������� ������.
			CObject* obj = (CObject*)Userdata::get<CObject>(L, 1, false);
			if (obj != NULL) {//obj->Remove();
				Command<COMMAND_DELETE_OBJECT>(CPools::GetObjectRef(obj));// ������� ������.
			}
			return 0;
		}
		else { throw "bad argument in function remove_obj"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int car_in_water(lua_State* L) {// �������� ���� � ����.
	try {
		if (LUA_TUSERDATA == lua_type(L, -1)) {// �������� �����.
			CVehicle* car = (CVehicle*)Userdata::get<CVehicle>(L, 1, false);
			bool checkinwanter = Command<COMMAND_IS_CAR_IN_WATER>(CPools::GetVehicleRef(car));
			Stack<bool>::push(L, checkinwanter);
			return 1;
		}
		else { throw "bad argument in function car_in_water"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int set_wanted(lua_State* L) {// �c�������� ������� �������.
	try {
		if (LUA_TNUMBER == lua_type(L, -1)) {// �������� �����.
			int wanted = Stack<int>::get(L, -1);
			Command<COMMAND_ALTER_WANTED_LEVEL>(CWorld::PlayerInFocus, wanted);
			return 0;
		}
		else { throw "bad argument in function set_wanted"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int clear_wanted(lua_State* L) {// ������ ������� �������.
	Command<COMMAND_CLEAR_WANTED_LEVEL>(CWorld::PlayerInFocus);
	return 0;
};

int remove_ped(lua_State* L) {// ������� ����.
	try {
		if (LUA_TUSERDATA == lua_type(L, -1)) {// �������� ���.
			CPed* ped = (CPed*)Userdata::get<CPed>(L, 1, false);
			Command<COMMAND_MARK_CHAR_AS_NO_LONGER_NEEDED>(CPools::GetPedRef(ped));// ������� ����.
			return 0;
		}
		else { throw "bad argument in function remove_ped"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int kill_ped(lua_State* L) {// ����� ����.
	try {
		if (LUA_TUSERDATA == lua_type(L, -1)) {// �������� �����.
			CPed* ped = (CPed*)Userdata::get<CPed>(L, 1, false);
			Command<COMMAND_EXPLODE_CHAR_HEAD>(CPools::GetPedRef(ped));// ����� ����.
			return 0;
		}
		else { throw "bad argument in function kill_ped"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int setpedcoordes(lua_State* L) {// ���������� ���������� ��� ����.
	try {
		if (LUA_TUSERDATA == lua_type(L, -4) && LUA_TNUMBER == lua_type(L, -3) && LUA_TNUMBER == lua_type(L, -2)
			&& LUA_TNUMBER == lua_type(L, -1)) {//�����.

			CPed* ped = (CPed*)Userdata::get<CPed>(L, 1, false);

			float x = Stack<float>::get(L, 2);
			float y = Stack<float>::get(L, 3);
			float z = Stack<float>::get(L, 4);
			CPed* player = FindPlayerPed();// ����� ������  
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
	catch (const char* x) { writelog(x); }// �������� ������ � ����.
	return 0;
};
int setobj�coordes(lua_State* L) {// ���������� ���������� ��� ������.
	try {
		if (LUA_TUSERDATA == lua_type(L, -4) && LUA_TNUMBER == lua_type(L, -3) && LUA_TNUMBER == lua_type(L, -2)
			&& LUA_TNUMBER == lua_type(L, -1)) {//������.

			CObject* obj = (CObject*)Userdata::get<CObject>(L, 1, false);// �������� ��������� �� ������.

			float x = Stack<float>::get(L, 2);
			float y = Stack<float>::get(L, 3);
			float z = Stack<float>::get(L, 4);
			Command<COMMAND_SET_OBJECT_COORDINATES>(CPools::GetObjectRef(obj), x, y, z);
			return 0;

		}
		else { throw "bad argument in function setobj�coordes"; }
	}
	catch (const char* x) { writelog(x); }// �������� ������ � ����.
	return 0;
};
int create_marker_obj(lua_State* L) {//������� ������ ��� ��������.
	int marker;
	try {
		if (LUA_TUSERDATA == lua_type(L, -1)) {// ��������� �� ������.
			CObject* obj = (CObject*)Userdata::get<CObject>(L, 1, false);// �������� ��������� �� ������.
			Command<COMMAND_ADD_BLIP_FOR_OBJECT>(CPools::GetObjectRef(obj), &marker);
			markeron.emplace(marker, L);// �������� � map ��� ��������.
			Stack<int>::push(L, marker);// ��������� � ����.  
			return 1;
		}
		else { throw "bad argument in function create_marker_obj"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int move_obj(lua_State* L) {//������� ������.
	try {
		if (LUA_TUSERDATA == lua_type(L, 1)) {// ��������� �� ������.
			CObject* obj = (CObject*)Userdata::get<CObject>(L, 1, false);// �������� ��������� �� ������.
			float x = Stack<float>::get(L, 2); float y = Stack<float>::get(L, 3);
			float z = Stack<float>::get(L, 4);
			float speedx = Stack<float>::get(L, 5);
			float speedy = Stack<float>::get(L, 6);
			float speedz = Stack<float>::get(L, 7);
			int collision_check = Stack<int>::get(L, 8);
			Command<COMMAND_SLIDE_OBJECT>(CPools::GetObjectRef(obj), x, y, z, speedx, speedy, speedz, collision_check);

			return 0;
		}
		else { throw "bad argument in function move_obj"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int move_rotate(lua_State* L) {//������� ������.
	try {
		if (LUA_TUSERDATA == lua_type(L, 1)) {// ��������� �� ������.
			lua_settop(L, 4);
			CObject* obj = (CObject*)Userdata::get<CObject>(L, 1, false);// �������� ��������� �� ������.
			float Angle1 = Stack<float>::get(L, 2); float Angle2 = Stack<float>::get(L, 3);
			int flag = Stack<int>::get(L, 4);
			Command<COMMAND_ROTATE_OBJECT>(CPools::GetObjectRef(obj), Angle1, Angle1, flag);

			lua_settop(L, 0);
			return 0;
		}
		else { throw "bad argument in function move_obj"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int getobjcoordes(lua_State* L) {// �������� ���������� �������.
	try {
		if (LUA_TUSERDATA == lua_type(L, -1)) {// ��������� �� ����.
			CObject* obj = (CObject*)Userdata::get<CObject>(L, 1, false);
			Stack<double>::push(L, obj->GetPosition().x);// ��������� � ����.
			Stack<double>::push(L, obj->GetPosition().y);// ��������� � ����.
			Stack<double>::push(L, obj->GetPosition().z);// ��������� � ����.
			return 3;
		}// �������� ���������� �����.

		else { throw "bad argument in function getcarcoordes"; }
	}
	catch (const char* x) { writelog(x); }
};
int ped_in_point_in_radius(lua_State* L) {// ��������� ��������� ��� � ����������� � ��������.
	try {
		if (LUA_TUSERDATA == lua_type(L, -7) && LUA_TNUMBER == lua_type(L, -6) && LUA_TNUMBER == lua_type(L, -5) &&
			LUA_TNUMBER == lua_type(L, -4) && LUA_TNUMBER == lua_type(L, -3) && LUA_TNUMBER == lua_type(L, -2)
			&& LUA_TNUMBER == lua_type(L, -1)) {//������.

			CPed* ped = (CPed*)Userdata::get<CPed>(L, 1, false);

			float x = Stack<float>::get(L, 2);
			float y = Stack<float>::get(L, 3);
			float z = Stack<float>::get(L, 4);
			float rx = Stack<float>::get(L, 5);
			float ry = Stack<float>::get(L, 6);
			float rz = Stack<float>::get(L, 7);
			CPed* player = FindPlayerPed();// ����� ������  
			if (ped != player) {
				bool point = Command<COMMAND_LOCATE_CHAR_ANY_MEANS_3D >(CPools::GetPedRef(ped), x, y, z, rx, ry, rz);
				lua_settop(L, 0);
				Stack<bool>::push(L, point);
				return 1;
			}
			else {
				bool point = Command<COMMAND_LOCATE_PLAYER_ANY_MEANS_3D>(CWorld::PlayerInFocus, x, y, z, rx, ry, rz);
				lua_settop(L, 0);// �������� ����.
				Stack<bool>::push(L, point);
				return 1;
			}
		}
		else { throw "bad argument in function ped_in_point_in_radius"; }
	}
	catch (const char* x) { writelog(x); }// �������� ������ � ����.
	return 0;
};
int car_in_point_in_radius(lua_State* L) {// ��������� ��������� ���� � ����������� � ��������.
	static int delay = 0;
	try {
		if (LUA_TUSERDATA == lua_type(L, -7) && LUA_TNUMBER == lua_type(L, -6) && LUA_TNUMBER == lua_type(L, -5) &&
			LUA_TNUMBER == lua_type(L, -4) && LUA_TNUMBER == lua_type(L, -3) && LUA_TNUMBER == lua_type(L, -2)
			&& LUA_TNUMBER == lua_type(L, -1)) {//������.

			if (delay == 0) {
				delay = iters; 
				Stack<bool>::push(L, false);
				return 1;
			}
			else {
				if (iters - delay > 5) {
					delay = 0;
					CVehicle* car = (CVehicle*)Userdata::get<CVehicle>(L, 1, false);

					float x = Stack<float>::get(L, 2);
					float y = Stack<float>::get(L, 3);
					float z = Stack<float>::get(L, 4);
					float rx = Stack<float>::get(L, 5);
					float ry = Stack<float>::get(L, 6);
					float rz = Stack<float>::get(L, 7);
					lua_pop(L, lua_gettop(L));
					Stack<bool>::push(L, Command<COMMAND_LOCATE_CAR_3D>(CPools::GetVehicleRef(car), x, y, z, rx, ry, rz));
					return 1;
				}
			}
		}
		else { throw "bad argument in function car_in_point_in_radius"; }
	}
	catch (const char* x) { writelog(x); }// �������� ������ � ����.
	return 0;
};

void dellod() {// ������� ��� ������.
	string path = "lualoader\\log.txt";
	fstream f1; {f1.open(path, fstream::in | fstream::out | fstream::app);
	if (f1.is_open()) {// ���� ���� ���� �������.
		f1.close();	remove("lualoader\\log.txt");
	}
	}
}
int cleanstl() {//�������� �������� �� ���� stl.
	if (!markeron.empty()) {// ���� �� �����.
		markeron.clear();// �������
	}
	if (!spheres.empty()) {// ���� �� �����.
		spheres.clear();// �����.
	}
	if (!pickupsids.empty()) {// ���� �� �����.
		pickupsids.clear();//������.
	}
	if (!mapcars.empty()) {// ���� �� �����.
		mapcars.clear();//����.
	}
	if (!mapobjs.empty()) {// ���� �� �����.
		mapobjs.clear();//������.
	}

	return 0;
};

int getflagmission(lua_State* L) {// �������� ����� ������.
	unsigned int& OnAMissionFlag = *(unsigned int*)0x978748;
	bool getflagmission = (CTheScripts::ScriptSpace[OnAMissionFlag]);
	Stack<bool>::push(L, getflagmission);
	return 1;
};
int setflagmission(lua_State* L) {// �c�������� ����� ������.
	unsigned int& OnAMissionFlag = *(unsigned int*)0x978748;
	try {
		if (LUA_TNUMBER == lua_type(L, -1)) {// �������� �����.
			int flag = Stack<int>::get(L, -1);
			CTheScripts::ScriptSpace[OnAMissionFlag] = flag;
			return 0;
		}
		else { throw "bad argument in function setflagmission"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int showtext(lua_State* L) {// ����� ������� ������ �� �����.
	try {
		if (LUA_TSTRING == lua_type(L, -3) && LUA_TNUMBER == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {//������.
			const char* c = lua_tostring(L, -3);
			int time = lua_tointeger(L, -2);// ����� ������ ������.
			int style = lua_tointeger(L, -1);// ����� ������.
			wchar_t* str = getwchat(c);
			CMessages::AddBigMessage(str, time, style);/*0 ��������, ������� ��� ������ ��������,
			1 ������� ��� �������� ������, 2 ������� ��������*/
			return 0;
		}
		else { throw "bad argument in function showtext"; }
	}
	catch (const char* x) { writelog(x); }// �������� ������ � ����.
	return 0;
};

int remove_blip(lua_State* L) {// ������� ����� � �����.
	try {
		if (LUA_TNUMBER == lua_type(L, -1)) {// �������� �����.
			int blip = Stack<int>::get(L, -1);
			Command<COMMAND_REMOVE_BLIP>(blip);// ������� ����� �� �����.
			return 0;
		}
		else { throw "bad argument in function remove_blip"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int createblip(lua_State* L) {// ������� ����� �����.
	try {
		if (LUA_TNUMBER == lua_type(L, -4) && LUA_TNUMBER == lua_type(L, -3) &&
			LUA_TNUMBER == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {// �������� �����.

			int t = Stack<int>::get(L, -4);// id ����� �� �����.
			float x = Stack<float>::get(L, -3); float y = Stack<float>::get(L, -2);
			float z = Stack<float>::get(L, -1); CVector p = { x, y, z }; int point;
			Command<COMMAND_ADD_SHORT_RANGE_SPRITE_BLIP_FOR_CONTACT_POINT>(p.x, p.y, p.z, t, &point);
			Stack<int>::push(L, point);// ��������� � ���� � �������� �� ����� �����
			markeron.emplace(point, L);// �������� � map ��� ��������.
			return 1;
		}// int

		else { throw "bad argument in function createblip"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int createmarker(lua_State* L) {// ������� ������ �� �����.
	try {
		if (LUA_TNUMBER == lua_type(L, -5) && LUA_TNUMBER == lua_type(L, -4) && LUA_TNUMBER == lua_type(L, -3) &&
			LUA_TNUMBER == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {// �������� �����.
			int t = Stack<int>::get(L, 1);// ��� ������.
			int size = Stack<int>::get(L, 2);// ������ ������� �� �����.
			float x = Stack<float>::get(L, 3); float y = Stack<float>::get(L, 4);
			float z = Stack<float>::get(L, 5); CVector p = { x, y, z };
			int point;
			Command<COMMAND_ADD_BLIP_FOR_COORD_OLD>(p.x, p.y, p.z, t, size, &point);
			markeron.emplace(point, L);// �������� � map ��� ��������.
			Stack<int>::push(L, point);// ��������� � ���� � �������� �� ����� �����
			return 1;
		}// int

		else { throw "bad argument in function createblip"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int play_sound(lua_State* L) {// ��������� �������.
	try {
		if (LUA_TNUMBER == lua_type(L, -1)) {// �������� �����.
			int music = Stack<int>::get(L, -1);// �������� ����� �������.
			Command<COMMAND_PLAY_MISSION_PASSED_TUNE>(music);
			return 0;
		}
		else { throw "bad argument in function play_sound"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int create_marker_pickup(lua_State* L) {// ������� ������ ��� �������.
	try {
		int marker;
		if (LUA_TNUMBER == lua_type(L, 1)) {// �������� �����.
			int pickup = Stack<int>::get(L, 1);// �������� ����� �������.
			Command<COMMAND_ADD_BLIP_FOR_PICKUP>(pickup, &marker);
			markeron.emplace(marker, L);// �������� � map ��� ��������.
			Stack<int>::push(L, marker);// ��������� � ���� � �������� �� ����� �����
			return 1;
		}
		else { throw "bad argument in function play_sound"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int play_voice(lua_State* L) {
	try {
		if (LUA_TSTRING == lua_type(L, -1)) {// �������� �����.
			const char* voice = Stack<const char*>::get(L, -1);
			Command<COMMAND_LOAD_MISSION_AUDIO>(1, voice);// ��������� �������.
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
	catch (const char* x) { writelog(x); }
	return 0;
};
int createped(lua_State* L) {// ������� ����.
	try {
		if (LUA_TNUMBER == lua_type(L, -5) && LUA_TNUMBER == lua_type(L, -4) && LUA_TNUMBER == lua_type(L, -3) &&
			LUA_TNUMBER == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {// �������� �����.
			int model = Stack<int>::get(L, -5);// ������ ����
			int type = Stack<int>::get(L, -4);// ��� ����.
			float x = Stack<float>::get(L, -3); float y = Stack<float>::get(L, -2);
			float z = Stack<float>::get(L, -1); CVector pos = { x, y, z };
			CPed* ped = nullptr;
			Command<COMMAND_CREATE_CHAR>(type, model, pos.x, pos.y, pos.z, &ped);
			mappeds.emplace(ped, L);// �������� � list ��� �����.
			lua_rawsetp(L, LUA_REGISTRYINDEX, "cped");  /* ��� ���� � ������� ����� k.  */

			const void* p = lua_topointer(L, -1);
			vector_for_ped.push_back(p);
			Stack<CPed*>::push(L, ped);// ��������� � ���� � �������� �� ����� �����.
			return 1;
		}// int

		else { throw "bad argument in function createped"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int isped(lua_State* L) {// �������� ��� ���?.
	try {
		if (LUA_TUSERDATA == lua_type(L, -1)) {// �������� �����.
						//	if (Stack<CPed>::is_a(L, 1))
			{
				Stack<bool>::push(L, true);
				return 1;
			}
			//else {
			//	Stack<bool>::push(L, false);
			//	return 1;
			//}
		}
		else { throw "bad argument in function isped"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int isvehicle(lua_State* L) {// �������� ��� ���������?.
	try {
		if (LUA_TUSERDATA == lua_type(L, -1)) {// �������� ������.
			const void* vehicle = lua_topointer(L, -1);
			int L1 = (int)& L;
			int vehicle1 = (int)& vehicle;
			int cvehicle = L1 + vehicle1;// ����� ������� ��������� � ��������� �� ������.

			lua_pushinteger(L, vehicle1);  /*��������� �����, ������� �������� ������ � ����. */
			lua_gettable(L, LUA_REGISTRYINDEX);  /* �������� ������� � �������� ����� ����� � -1 */
			const char* clas = lua_tostring(L, -1);// ��� ����� �����.������ � ������� �����.
			const char* st = "cvehicle";
			if (strcmp(clas, st) == 0) {
				Stack<bool>::push(L, true);
				return 1;
			}
			else {
				Stack<bool>::push(L, false);
				return 1;
			}
		}
		else { throw "bad argument in function isvehicle"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int isobject(lua_State* L) {// �������� ��� ������?.
	try {
		if (LUA_TUSERDATA == lua_type(L, -1)) {// �������� ������.
			const void* obj = lua_topointer(L, -1);
			lua_gettable(L, LUA_REGISTRYINDEX);  /* �������� ������� � �������� ����� ����� � -1 */

			const char* clas = lua_tostring(L, -1);// ��� ����� �����.������ � ������� �����.
			const char* st = "cobject";
			if (strcmp(clas, st) == 0) {
				Stack<bool>::push(L, true);
				return 1;
			}
			else {
				Stack<bool>::push(L, false);
				return 1;
			}
		}
		else { throw "bad argument in function isobject"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int fade(lua_State* L) {//���������, ������������.
	try {
		if (LUA_TNUMBER == lua_type(L, -2) && (LUA_TNUMBER == lua_type(L, -1))) {
			int lihgt = Stack<int>::get(L, 1);// �����.
			int time = Stack<int>::get(L, 2);// ���� ����.
			Command<COMMAND_DO_FADE>(time, lihgt);// ���������.
			return 0;
		}
		else { throw "bad argument in function fade"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int setsizemarker(lua_State* L) {//���������� ������ �������.
	try {
		if (LUA_TNUMBER == lua_type(L, -2) && (LUA_TNUMBER == lua_type(L, -1))) {
			int marker = Stack<int>::get(L, 1);// �����.
			int size = Stack<int>::get(L, 2);// ���� ����.
			Command<COMMAND_CHANGE_BLIP_SCALE>(marker, size);
			return 0;
		}
		else { throw "bad argument in function fade"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int draw_corona(lua_State* L) {// ������� ������.
	try {
		if (LUA_TTABLE == lua_type(L, -1)) {// �������� ������� �� 9 ���������.
			for (int i = 1; i < 10; i++){
			lua_pushinteger(L, i);
			lua_gettable(L, -2); lua_insert(L, i);
			}
			float radius = Stack<float>::get(L, 1);// ������ ������.
			int type = Stack<int>::get(L, 2);// ���.
			float glow_flare = Stack<int>::get(L, 3); // ��������. 
			int red = Stack<int>::get(L, 4);// ����� 
			int green = Stack<int>::get(L, 5);
			int blue = Stack<int>::get(L, 6);
			float x = Stack<float>::get(L, 7); // ����������.
			float y = Stack<float>::get(L, 8);
			float z = Stack<float>::get(L, 9);
			lua_pop(L, lua_gettop(L));

			CVector pos = { x,y,z };

			Command<COMMAND_DRAW_CORONA>(pos.x, pos.y, pos.z, radius, type, glow_flare, red, green, blue);
			return 0;
		}// int

		else { throw "bad argument in function draw_corona"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int sound_coordinate(lua_State* L) {// ��������� ���� � �����������
	try {
		if (LUA_TNUMBER == lua_type(L, -4) && LUA_TNUMBER == lua_type(L, -3) &&
			LUA_TNUMBER == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {// �������� �����.

			int sound = Stack<int>::get(L, 1);// id �����.
			float x = Stack<float>::get(L, 2); float y = Stack<float>::get(L, 3);
			float z = Stack<float>::get(L, 4);
			Command<COMMAND_ADD_ONE_OFF_SOUND>(x, y, z, sound);// ��������� ���� � �����������
			lua_settop(L, 0);
			return 0;
		}// int

		else { throw "bad argument in function sound_coordinate"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int show_text_styled(lua_State* L) {// ������� ������� �����.
	try {
		if (LUA_TSTRING == lua_type(L, -3) && LUA_TNUMBER == lua_type(L, -2)
			&& LUA_TNUMBER == lua_type(L, -1)) {// �������� �����.
			const char* text = Stack<const char*>::get(L, 1);// �����.
			int time = Stack<int>::get(L, 2);	int type = Stack<int>::get(L, 3);
			Command<COMMAND_PRINT_BIG>(text, time, type);// ����� ��� ������ �����.
			lua_settop(L, 0);// �������� �����	
			return 0;
		}

		else { throw "bad argument in function show_text_styled"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int setcardrive(lua_State* L) {// ���������� �������� ��� ����.
	try {
		if (LUA_TUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {//������.

			CVehicle* car = (CVehicle*)Userdata::get<CVehicle>(L, 1, false);
			int model = Stack<int>::get(L, 2);// ������ ����.
			int type = Stack<int>::get(L, 3);// ��� ����.
			CPed* ped = nullptr;
			Command<COMMAND_CREATE_CHAR_INSIDE_CAR>(CPools::GetVehicleRef(car), type, model, &ped);
			car->m_autoPilot.m_nCarMission = MISSION_NONE;
			Stack<CPed*>::push(L, ped);
			return 1;
		}
		else { throw "bad argument in function setcardrive"; }
	}
	catch (const char* x) { writelog(x); }// �������� ������ � ����.
	return 0;
};
int setcarpassenger(lua_State* L) {// ���������� ��������� ��� ����.
	try {
		if (LUA_TUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)
			&& LUA_TNUMBER == lua_type(L, 3) && LUA_TNUMBER == lua_type(L, 3)) {//�����.

			CVehicle* car = (CVehicle*)Userdata::get<CVehicle>(L, 1, false);
			int model = Stack<int>::get(L, 2);
			int type = Stack<int>::get(L, 3);
			int place = Stack<int>::get(L, 4);//����� ���������.
			CPed* ped = nullptr;
			Command<COMMAND_CREATE_CHAR_AS_PASSENGER>(CPools::GetVehicleRef(car), type, model, place, &ped);
			Stack<CPed*>::push(L, ped);
			return 1;
		}
		else { throw "bad argument in function setcarpassenger"; }
	}
	catch (const char* x) { writelog(x); }// �������� ������ � ����.
	return 0;
};
int setcarfirstcolor(lua_State* L) {// ���������� ������ ���� ����.
	try {
		if (LUA_TUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = (CVehicle*)Userdata::get<CVehicle>(L, 1, false);// �������� ��������� �� ����.
			int firstcolor = Stack<int>::get(L, 2);
			car->m_nPrimaryColor = firstcolor;// ���������� ������ ���� ����.
			return 0;
		}
		else { throw "bad argument in function setcarfirstcolor"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int setcarseconscolor(lua_State* L) {// ���������� ������ ���� ����.
	try {
		if (LUA_TUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = (CVehicle*)Userdata::get<CVehicle>(L, 1, false);// �������� ��������� �� ����.
			int secondcolor = Stack<int>::get(L, 2);
			car->m_nSecondaryColor = secondcolor;
			return 0;
		}// ���������� ������ ���� ����.

		else { throw "bad argument in function setcarseconscolor"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int set_traffic(lua_State* L) {// ���������� ������ ����������.
	try {
		if (LUA_TNUMBER == lua_type(L, -1)) {// �������� �����.
			float trafic = Stack<float>::get(L, -1);
			Command<COMMAND_SET_CAR_DENSITY_MULTIPLIER>(trafic);
			return 0;
		}
		else { throw "bad argument in function set_traffic"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int car_explode(lua_State* L) {// �������� ����.
	try {
		if (LUA_TUSERDATA == lua_type(L, 1)) {// ��������� �� ����. 
			CVehicle* car = (CVehicle*)Userdata::get<CVehicle>(L, 1, false);// �������� ��������� �� ����.
			Command<COMMAND_EXPLODE_CAR>(CPools::GetVehicleRef(car));
			return 0;
		}
		else { throw "bad argument in function car_explode"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int is_car_stopped(lua_State* L) {// ���� ������������? 
	try {
		if (LUA_TUSERDATA == lua_type(L, 1)) {// ��������� �� ����. 
			CVehicle* car = (CVehicle*)Userdata::get<CVehicle>(L, 1, false);// �������� ��������� �� ����.
			Stack<bool>::push(L, Command<COMMAND_IS_CAR_STOPPED>(CPools::GetVehicleRef(car)));
			return 1;
		}
		else { throw "bad argument in function is_car_stopped"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int setclock(lua_State* L) {//  ������ �����.
	try {
		if (LUA_TNUMBER == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {//�����.

			int hours = Stack<int>::get(L, 1);// ����.
			int minutes = Stack< int>::get(L, 2);// ������.

			CClock::SetGameClock(hours, minutes);// ������ �����.
			return 0;
		}
		else { throw "bad argument in function create_explosion"; }
	}
	catch (const char* x) { writelog(x); }// �������� ������ � ����.
	return 0;
}

int create_explosion(lua_State* L) {// ������� ����� �� �����������.
	try {
		if (LUA_TNUMBER == lua_type(L, -4) && LUA_TNUMBER == lua_type(L, -3) && LUA_TNUMBER == lua_type(L, -2)
			&& LUA_TNUMBER == lua_type(L, -1)) {//�����.
			
			int tipe = Stack<int>::get(L, 1);// ��� ������.
			double x = Stack<double>::get(L, 2);
			double y = Stack<double>::get(L, 3);
			double z = Stack<double>::get(L, 4);
			/*
			0 ����������� ������� �����, ������������ ���� ����� Highslide JS
			1 ����� ��� �� �������� ��������. ��-����� ����� ���� ��������� ����� ���� � ���� �����. ������� ����� ��������� ��, ��� ������ ���� ����� ��������� ���������� ��� ����������� ������ � ����� �����. Highslide JS
			2 ����� �� ��� 0 Highslide JS
			3 ����� �� ��� 0 Highslide JS
			4 ����-��� �������� �����. ������� ����� � ����, ��� � ����� 4 � 5 �� ������ ������ ������. ��� ��� ������������ ���� ��� � ������� ������ ��� ���������� ���� �������� ������� 0565 (������ ����) Highslide JS
			5 ������ �� ��� 4 Highslide JS
			6 ������� �����, ���� �� ����� ������� Highslide JS
			7 ���� �������� ��� ��� 6 Highslide JS
			8 ����� ���, ������� ���, �������� ������ ����, �������� ��������. Highslide JS
			9 �� �� �����, ��� � ��� 8 Highslide JS
			10 ������ � ����� 6 ����� ������� ����� Highslide JS
			11 ��� ��� 0, �� ���� ������ Highslide JS
			12 ����� ��������� �����, ��� �� ������ RC-�������. Highslide JS
			13+ ����������� � ����������� ����, ��-��������, �� �������������� �����: ������ ��� �����, ��� �������.
			*/
			Command<COMMAND_ADD_EXPLOSION>(x, y, z, tipe);
			return 0;
		}
		else { throw "bad argument in function create_explosion"; }
	}
	catch (const char* x) { writelog(x); }// �������� ������ � ����.
	return 0;
};
int set_status_engine(lua_State* L) {// ���������� ��������� ��������� ����.
	try {
		if (LUA_TUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = (CVehicle*)Userdata::get<CVehicle>(L, 1, false);// �������� ��������� �� ����.
			int switcher = Stack<int>::get(L, 2);
			Command<COMMAND_BOAT_STOP>(CPools::GetVehicleRef(car), switcher);// 
			return 0;
		}//

		else { throw "bad argument in function set_status_engine"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int player_defined(lua_State* L) {// ����� ����������.
	try {
		if (LUA_TUSERDATA == lua_type(L, -1)) {// ��������� �� ������.
			CPed* ped = (CPed*)Userdata::get<CPed>(L, 1, false);// �������� ��������� �� ������.
			float health = ped->m_fHealth;
			if (health > 1.0f) {
				Stack<bool>::push(L, true);
				return 1;
			}
			else {
				Stack<bool>::push(L, false);
				return 1;
			}
		}
		else { throw "bad argument in function player_defined"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int arrested(lua_State* L) { // ����� ���������?
	if (CWorld::Players[CWorld::PlayerInFocus].m_nPlayerState == PLAYERSTATE_HASBEENARRESTED) {
		Stack<bool>::push(L, true);
		return 1;
	}
	else {
		Stack<bool>::push(L, false);
		return 1;
	}
};

int setcarcoordes(lua_State* L) {// ���������� ���������� ����.
	try {
		if (LUA_TUSERDATA == lua_type(L, -4) && LUA_TNUMBER == lua_type(L, -3) && LUA_TNUMBER == lua_type(L, -2)
			&& LUA_TNUMBER == lua_type(L, -1)) {//�����.

			CVehicle* car = (CVehicle*)Userdata::get<CVehicle>(L, 1, false);

			float x = Stack<float>::get(L, 2);
			float y = Stack<float>::get(L, 3);
			float z = Stack<float>::get(L, 4);
			Command<COMMAND_SET_CAR_COORDINATES>(CPools::GetVehicleRef(car), x, y, z);// ���������� ���������� ����.
		}
		else { throw "bad argument in function setcarcoordes"; }
	}
	catch (const char* x) { writelog(x); }// �������� ������ � ����.
	return 0;
};
int is_car_stuck(lua_State* L) {//03CE: car 12@ stuck ���� ������ ��������.
	try {
		if (LUA_TUSERDATA == lua_type(L, -1)) {// �������� �����.
			CVehicle* car = (CVehicle*)Userdata::get<CVehicle>(L, 1, false);
			bool checkstuck = Command<COMMAND_IS_CAR_STUCK>(CPools::GetVehicleRef(car));
			Stack<bool>::push(L, checkstuck);
			return 1;
		}
		else { throw "bad argument in function is_car_stuck"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int is_car_upsidedown(lua_State* L) {//01F4: car 12@ flipped ���� ������ �����������.
	try {
		if (LUA_TUSERDATA == lua_type(L, -1)) {// �������� �����.
			CVehicle* car = (CVehicle*)Userdata::get<CVehicle>(L, 1, false);
			bool checkupsidedown = Command<COMMAND_IS_CAR_UPSIDEDOWN>(CPools::GetVehicleRef(car));
			Stack<bool>::push(L, checkupsidedown);
			return 1;
		}
		else { throw "bad argument in function is_car_upsidedown"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int is_car_upright(lua_State* L) {// 020D: car 12@ flipped ���� ��������� ���������� ����������.
	try {
		if (LUA_TUSERDATA == lua_type(L, -1)) {// �������� �����.
			CVehicle* car = (CVehicle*)Userdata::get<CVehicle>(L, 1, false);
			bool upright = Command<COMMAND_IS_CAR_UPRIGHT>(CPools::GetVehicleRef(car));
			Stack<bool>::push(L, upright);
			return 1;
		}
		else { throw "bad argument in function is_car_upright"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int find_road_for_car(lua_State* L) {// ����� ������.
	try {
		if (LUA_TNUMBER == lua_type(L, -3) && LUA_TNUMBER == lua_type(L, -2)
			&& LUA_TNUMBER == lua_type(L, -1)) {//�����.

			float x1 = Stack<float>::get(L, 1);
			float y1 = Stack<float>::get(L, 2);
			float z1 = Stack<float>::get(L, 3);
			double x, y, z;
			Command<COMMAND_GET_CLOSEST_CAR_NODE>(x1, y1, z1, x, y, z);// ����� ������.
			Stack<double>::push(L, x);
			Stack<double>::push(L, y);
			Stack<double>::push(L, z);
			return 3;
		}
		else { throw "bad argument in function find_road_for_car"; }
	}
	catch (const char* x) { writelog(x); }// �������� ������ � ����.
	return 0;
};
int setcarstrong(lua_State* L) {// ������� ���� ����������.
	try {
		if (LUA_TUSERDATA == lua_type(L, -2) && LUA_TNUMBER == lua_type(L, -1)) {//��������� �� ����.

			CVehicle* car = (CVehicle*)Userdata::get<CVehicle>(L, 1, false);

			int value = Stack<int>::get(L, 2);
			if (value == 1) {
				Command<COMMAND_SET_CAR_STRONG>(CPools::GetVehicleRef(car), true);// ����� ������.
				return 0;
			}
			if (value == 0) {

				Command<COMMAND_SET_CAR_STRONG>(CPools::GetVehicleRef(car), false);// ����� ������.
				return 0;
			}
		}
		else { throw "bad argument in function setcarstrong"; }
	}
	catch (const char* x) { writelog(x); }// �������� ������ � ����.
	return 0;
};
int putincar(lua_State* L) {// ����������� ���� � ����.
	try {
		if (LUA_TUSERDATA == lua_type(L, 2) && LUA_TUSERDATA == lua_type(L, -1)) {//�����.

			CPed* ped = (CPed*)Userdata::get<CPed>(L, 1, false);
			CVehicle* car = (CVehicle*)Userdata::get<CVehicle>(L, 2, false);
			ped->SetObjective(OBJECTIVE_ENTER_CAR_AS_DRIVER, car);
			ped->WarpPedIntoCar(car); // ����������� ���� � ����

			//CPed* player = FindPlayerPed();// ����� ������  
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
	catch (const char* x) { writelog(x); }// �������� ������ � ����.
	return 0;
};
int ped_attack_car(lua_State* L) {// ��� ������� ����.
	try {
		if (LUA_TUSERDATA == lua_type(L, 2) && LUA_TUSERDATA == lua_type(L, 1)) {//�����.

			CPed* ped = (CPed*)Userdata::get<CPed>(L, 1, false);
			CVehicle* car = (CVehicle*)Userdata::get<CVehicle>(L, 2, false);
			ped->SetObjective(OBJECTIVE_DESTROY_CAR, car); //���������� ������ 01D9
			return 0;

		}
		else { throw "bad argument in function ped_attack_car"; }
	}
	catch (const char* x) { writelog(x); }// �������� ������ � ����.
	return 0;
};
int checkcheat(lua_State* L) {//��� ��� ������?.
	try {
		if (LUA_TSTRING == lua_type(L, -1)) {// ���� ������
			string cheat = Stack<string>::get(L, 1);// 
			if (-1 != cheatstr.find(cheat)) {
				cheatstr.clear();
				Stack<bool>::push(L, true); cheatstr.erase(0, 1);
				return 1;
			}
			else {
				Stack<bool>::push(L, false);
				return 1;
			}
		}
		else { throw "bad argument in function getpedarmour option of the player"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
}
int destroy(lua_State* L) {// �������� �������� �� ������ ��� ������������ �������. 
	int i;
	map<int, lua_State*>::iterator it;

	for (auto it = markeron.begin(); it != markeron.end(); ++it) {
		if (L == it->second) {
			i = it->first;
			Command<COMMAND_REMOVE_BLIP>(i);//������� ������.
			markeron.erase(i);
		}
	}

	for (auto it = spheres.begin(); it != spheres.end(); ++it) {
		if (L == it->second) {
			i = it->first;
			Command<COMMAND_REMOVE_SPHERE>(i);// ������� �����.
			spheres.erase(i);
		}
	}

	//map<int, lua_State*>::iterator pick;

	for (auto pick = pickupsids.begin(); pick != pickupsids.end(); ++pick) {
		if (L == pick->second) {
			i = pick->first;
			Command<COMMAND_REMOVE_PICKUP>(i);// ������� �����.
			pickupsids.erase(i);
		}
	}

	for (auto pick = mappeds.begin(); pick != mappeds.end(); ++pick) {
		if (L == pick->second) {
			CPed* ped = pick->first;
			Command<COMMAND_MARK_CHAR_AS_NO_LONGER_NEEDED>(CPools::GetPedRef(ped));// ������� ����.
			mappeds.erase(pick);
		}
	}

	for (auto cars = mapcars.begin(); cars != mapcars.end(); ++cars) {
		if (L == cars->second) {
			CVehicle* car = cars->first;
			Command<COMMAND_MARK_CAR_AS_NO_LONGER_NEEDED>(CPools::GetVehicleRef(car));// ������� ����.
			mapcars.erase(cars);
		}
	}

	for (auto objs = mapobjs.begin(); objs != mapobjs.end(); ++objs) {
		if (L == objs->second) {
			CObject* obj = objs->first;
			Command<COMMAND_DELETE_OBJECT>(CPools::GetObjectRef(obj));// ������� ������.
			mapobjs.erase(objs);
		}
	}

	return 0;
};

int game_font_print(lua_State* L) {// ������� ��������� ������� �����.
	try {
		if (LUA_TTABLE == lua_type(L, -1)) {// �������� ������� �� 9 ���������. 
			for (int i = 1; i < 12; i++) {
				lua_pushinteger(L, i);
				lua_gettable(L, -2); lua_insert(L, i);
			}
			lua_pop(L, 1);
				printgame = Stack<bool>::get(L, 1);//�������� ���������� �� ������
				strprintgame = Stack<string>::get(L, 2);
				abc_x = Stack<int>::get(L, 3);
				ord_y = Stack<int>::get(L, 4);
				font = Stack<int>::get(L, 5);
				spacing = Stack<float>::get(L, 6);
				size_x = Stack<float>::get(L, 7);
				size_y = Stack<float>::get(L, 8);
				red = Stack<int>::get(L, 9);

				blue = Stack<int>::get(L, 10);
				green = Stack<int>::get(L, 11);
				lua_pop(L, lua_gettop(L));
				//showstack(L);
			return 0;
		}

		else { throw "bad argument in function game_font_print"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int star_timer(lua_State* L) {
	try {
		if (LUA_TSTRING == lua_type(L, 1)) {// ���� �����
			const char* str = lua_tostring(L, 1);// ������.
			Command<COMMAND_DISPLAY_ONSCREEN_TIMER_WITH_STRING>(str);
			Stack<const char*>::push(L, str);
			return 1;
		}
		else { throw "bad argument in function  star_timer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int stop_timer(lua_State* L) {// ���������� ������.
	try {
		 if (LUA_TSTRING == lua_type(L, 1)) {// ���� �����
		    const char* str = lua_tostring(L, 1);// ������.
			static int timer = 13608;
			CUserDisplay::OnscnTimer.ClearClock(timer); //014F: stop_timer $3402
			Command<COMMAND_CLEAR_ONSCREEN_TIMER>(str);
			var_$3402 = 0;
			return 0;
		}
		else { throw "bad argument in function stop_timer"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int timer_donw(lua_State* L) {//  ������ �� ����������.
	try {
		if (LUA_TNUMBER == lua_type(L, 1) && LUA_TSTRING == lua_type(L, 2) && LUA_TNUMBER == lua_type(L, 3)) {
			int time = lua_tointeger(L, 1);// ���� �����
			const char* str1 = lua_tostring(L, 2);// ������.
			char* str = (char*)str1;
			var_$3402 = 1000 * time; //in CLEO $3402 = 120000

			int tipe = lua_tointeger(L, 3);// ���� �����
			static int timer = 13608;
	    	CUserDisplay::OnscnTimer.AddClock(timer, str, tipe);
			return 0;
		}
		else { throw "bad argument in function timer_donw"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int ped_frozen(lua_State* L) {// ���������� ��p���.
	try {
		if (LUA_TNUMBER == lua_type(L, 1)) {
			int status = lua_tointeger(L, 1);// ���� �����

			Command<COMMAND_SET_PLAYER_CONTROL>(CWorld::PlayerInFocus, status);

			return 0;
		}
		else { throw "bad argument in function timer_donw"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
int hold_cellphone(lua_State* L) {// ������� �������.
	try {
		if(LUA_TUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CPed* ped = (CPed*)Userdata::get<CPed>(L, 1, false);
			int status = lua_tointeger(L, 2);// ���� �����.
			Command<COMMAND_SET_CHAR_ANSWERING_MOBILE>(ped, status);
			return 0;
		}
		else { throw "bad argument in function hold_cellphone"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int car_lastweapondamage(lua_State* L) {// ����� ������, ������� ������� ���� ����.
	try {
		if (LUA_TUSERDATA == lua_type(L, 1)) {//�����.

			CVehicle* car = (CVehicle*)Userdata::get<CVehicle>(L, 1, false);
			unsigned char c = car->m_nLastWeaponDamage;
			int d = (int)c;
			Stack<int>::push(L, d);
			return 1;

		}
		else { throw "bad argument in function car_lastweapondamage"; }
	}
	catch (const char* x) { writelog(x); }// �������� ������ � ����.
	return 0;
};
int car_currentgear(lua_State* L) {// ������� �������� ����.
	try {
		if (LUA_TUSERDATA == lua_type(L, 1)) {//�����.

			CVehicle* car = (CVehicle*)Userdata::get<CVehicle>(L, 1, false);
			unsigned char c = car->m_nCurrentGear;
			int d = (int)c;
			Stack<int>::push(L, d);
			return 1;

		}
		else { throw "bad argument in function car_currentgear"; }
	}
	catch (const char* x) { writelog(x); }// �������� ������ � ����.
	return 0;
};
int getcar_model(lua_State* L) {// �������� ������ ����.
	try {
		if (LUA_TUSERDATA == lua_type(L, 1)) {//�����.

			CVehicle* car = (CVehicle*)Userdata::get<CVehicle>(L, 1, false);
			unsigned char c = car->m_nModelIndex;
			int d = (int)c;
			Stack<int>::push(L, d);
			return 1;

		}
		else { throw "bad argument in function car_currentgear"; }
	}
	catch (const char* x) { writelog(x); }// �������� ������ � ����.
	return 0;
};
int setcarsiren(lua_State* L) {// ���������� ������ ��� ����.
	try {
		if (LUA_TUSERDATA == lua_type(L, 1) && LUA_TNUMBER == lua_type(L, 2)) {
			CVehicle* car = (CVehicle*)Userdata::get<CVehicle>(L, 1, false);// �������� ��������� �� ����.
			int tipe = Stack<int>::get(L, 2);
			car->m_bSirenOrAlarm = tipe;
			return 0;
		}
		else { throw "bad argument in function setcarsiren"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};

int ped_car_as_passenger(lua_State* L) {// ��� ������� � ���� ��� ��������.
	try {
		if (LUA_TUSERDATA == lua_type(L, 2) && LUA_TUSERDATA == lua_type(L, 1)) {//�����.

			CPed* ped = (CPed*)Userdata::get<CPed>(L, 1, false);
			CVehicle* car = (CVehicle*)Userdata::get<CVehicle>(L, 2, false);
			ped->SetObjective(OBJECTIVE_ENTER_CAR_AS_PASSENGER, car); //���������� ������ 01D9
			return 0;

		}
		else { throw "bad argument in function ped_car_as_passenger"; }
	}
	catch (const char* x) { writelog(x); }// �������� ������ � ����.
	return 0;
};
int ped_car_as_driver(lua_State* L) {// ��� ������� � ���� ��� ��������.
	try {
		if (LUA_TUSERDATA == lua_type(L, 2) && LUA_TUSERDATA == lua_type(L, 1)) {//�����.

			CPed* ped = (CPed*)Userdata::get<CPed>(L, 1, false);
			CVehicle* car = (CVehicle*)Userdata::get<CVehicle>(L, 2, false);
			ped->SetObjective(OBJECTIVE_ENTER_CAR_AS_DRIVER, car); //��� ������� � ���� ��� ��������.
			return 0;

		}
		else { throw "bad argument in function ped_car_as_driver"; }
	}
	catch (const char* x) { writelog(x); }// �������� ������ � ����.
	return 0;
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
int newthread(lua_State* L) {// ����� �����.
	try {
		if (LUA_TFUNCTION == lua_type(L, 1)) {
			int stacksize = lua_gettop(L);
			//lua_pushinteger(L, stacksize);
			return lua_yield(L, stacksize);
		}
		else { throw "bad argument in function newthread"; }
	}
	catch (const char* x) { writelog(x); }
	return 0;
};
//static char x[256]; 
			//snprintf(x, 256, "%.d", iters);

			//wchar_t* str = getwchat(x);
			//CMessages::AddMessageJumpQ(str, 1000, 1);// ������� ��������� �� �����.




					/*		wchar_t* str = L"kjhb";
							CVector pos = { x,y,z };
							"�.03C3: set_timer_with_text_to 16@ type 0 text L'R_TIME'  // �pem� ?ohk?:
							Command<COMMAND_DISPLAY_ONSCREEN_TIMER_WITH_STRING>(10, 0, L'R_TIME');*/



//lua_State* copystack(lua_State* L); // ����������� ����� � �������� ������ ���������.
//const char* newname(char* str) {
	//
	//	if (!std::experimental::filesystem::exists("./threads")) {// ���� ��� ����� �������
	//		std::experimental::filesystem::create_directories("./threads");
	//	}
	//	char n[225] = "threads\\1.lua";
	//	fstream f0;
	//	f0.open(n, fstream::in | fstream::out | fstream::app);
	//
	//	if (f0.is_open()) {
	//		f0.close();
	//		remove(n);
	//	};// ������ ��������, ���� �� ����.
	//
	//	fstream f;	f.open(str);
	//	fstream f1;
	//	f1.open(n, fstream::in | fstream::out | fstream::app);
	//	vector<string>strf;
	//	if (f.is_open() && f1.is_open()) {
	//		string str10;
	//		int number = 0;
	//		while (!f.eof()) {
	//			number++;
	//			getline(f, str10);
	//			strf.push_back(str10);
	//		};
	//		int lastst = number - 1;
	//		for (int i = 0; i < number; i++)
	//		{
	//			if (i != lastst) { f1 << strf[i] + "\n"; }
	//			else { f1 << strf[i]; }
	//		}
	//	}; f.close(); f1.close();
	//	const char* wor = "threads\\1.lua";// (char*)newnane;
	//	return wor;
	//};
	//
	//void star(lua_State* L1) {
	//	lua_pcall(L1, 1, 0, 0);
	//};
	//static int g = 1;
	//		g++;
	//
	//void m(lua_State* L, const char* func, int args) {
	//	lua_State* L1 = lua_newthread(L);
	//
	//	int ret, ret1;//	this_thread::sleep_for(chrono::milliseconds(100));  
	//	lua_sethook(L, LUAHook, LUA_MASKCOUNT, 30);	// �������� ������� ��������, ������� ��������� ����� �������� �����
	//	lua_sethook(L1, LUAHook1, LUA_MASKCOUNT, 30);	// �������� ������� ��������, ������� ��������� ����� �������� �����
	//
	//	lua_getglobal(L, "main");
	//	ret1 = lua_resume(L, L1, 0);
	//	CWorld::Players[CWorld::PlayerInFocus].m_nMoney += ret1;// ���� ����� 	
	//	while (true) {
	//		if (ret == LUA_YIELD) {//&& ret1 != LUA_YIELD
	//	lua_getglobal(L1, func);
	//	ret = lua_resume(L1, L, args);//������ FUNC
	//			this_thread::sleep_for(chrono::milliseconds(10));
	//		}
	//			if (ret1 == LUA_OK) {
	//			CWorld::Players[CWorld::PlayerInFocus].m_nMoney += 100;// ���� ����� 	
	//			this_thread::sleep_for(chrono::milliseconds(10));
	//			break;
	//			
	//		}
	//		else { continue; }
	//		if (ret1 == LUA_YIELD) {
	//		//.	CWorld::Players[CWorld::PlayerInFocus].m_nMoney += 1;// ���� ����� 	
	//			lua_getglobal(L1, func);
	//			ret = lua_resume(L1, L, args); //this_thread::sleep_for(chrono::milliseconds(100));
	//		}
	//		if (ret == LUA_OK) {// ������� ���������� �������.
	//			break;
	//		}
	//
	//	};
	//};
	//
	//lua_State* cop(lua_State* L) {
	//	lua_State* L1 = luaL_newstate();
	//	luaL_openlibs(L1);// ������� ����. ����������.
	//	funs(L1);// ������ ���� �������.
	//
	//	int stacksize = lua_gettop(L);// ���-�� ��������� �  ����.	
	//	stacksize++;
	//	for (int i = 1; i < stacksize; i++) {
	//		if (LUA_TUSERDATA == lua_type(L, i)) {// �������� �����.
	//			const void* value = lua_topointer(L, i);// �������� �������������� ��������� �� �����.������.	
	//			int value1 = (int)& value;
	//			lua_pushinteger(L, value1);  /*��������� �����, ������� �������� ������ � ����. */
	//			lua_gettable(L, LUA_REGISTRYINDEX);  /* �������� ������� � �������� ����� ����� � -1 */
	//			int type1 = lua_type(L, -1);
	//			static char x[256];
	//			snprintf(x, 256, "type1 = %.d", type1);
	//			wchar_t* s1 = getwchat(x);
	//			CMessages::AddMessageJumpQ(s1, 3000, 0);
	//			if (LUA_TSTRING == lua_type(L, -1)) {
	//				CWorld::Players[CWorld::PlayerInFocus].m_nMoney += 10000;// ���� ����� 
	//				const char* clas = lua_tostring(L, -1);// ��� ����� �����.������ � ������� �����.
	//				lua_pop(L, 1);		//wchar_t* str = getwchat(clas);
	//				const char* st = "cped";//		CMessages::AddMessageJumpQ(str, 6000, 1);
	//				if (strcmp(clas, st) == 0) {
	//					CWorld::Players[CWorld::PlayerInFocus].m_nMoney += 1;// ���� ����� 
	//					CPed* ped = (CPed*)Userdata::get<CPed>(L, i, false);
	//					Stack<CPed*>::push(L1, ped);// ��������� � ���� ��������� �� ������.
	//					lua_pop(L, 1);
	//				}
	//			}
	//		}
	//	}
	//	return L1;
	//};
//lua_pushlightuserdata(L, L);  /*��������� �����, ������� �������� ������ � ����. */
//lua_gettable(L, LUA_REGISTRYINDEX);  /* �������� ������� � �������� ����� ����� � -1 */
//char const* luaname = lua_tostring(L, -1);//��� lua �������.
//wchar_t* str = getwchat(luaname);

//lua_State* L1 = luaL_newstate();
//luaL_openlibs(L1);
//funs(L1);// ������ ���� �������.

//int stacksize = lua_gettop(L);
//stacksize++;
//for (int i = 1; i < stacksize; i++) {
//	lua_pushvalue(L, i);// ���������� �� ������� �����.
//	lua_xmove(L, L1, 1);// ������� � L1 ��������� �������� L.
//};

//luastate.push_back(L1);// �������� ����� ��������� � list
////int args = lua_gettop(L1);
//		if (0 == luaL_loadfile(L1, luaname)) {// ������� lua ����.      
//			lua_pcall(L1, 0, 0, 0);// ������ �����.
//			lua_pushvalue(L1, 1);//����������� ����� �������, ��������� �� ������� �����.
//			std::thread t([=]() {lua_pcall(L1, args, 0, 0); });
//			t.detach();

//int timer(int time, int t) {
//	if (CTimer::m_snTimeInMilliseconds - time > t) {//t = 0; // �������� ������
//		return 0;
//	};
//	if (CTimer::m_snTimeInMilliseconds - time < t) {
//		this_thread::sleep_for(chrono::milliseconds(1));
//		timer(t, time);
//	}
//};
//int stacksize = lua_gettop(L);
//stacksize++;
//for (int i = 1; i < stacksize; i++) {
//	lua_pushvalue(L, i);// ���������� �� ������� �����.
//	lua_xmove(L, L1, 1);// ������� � L1 ��������� �������� L.
//};
//luastate.push_back(L1);// �������� ����� ��������� � list
//int args = lua_gettop(L1);
//int args = lua_gettop(L); args++;
//lua_State* L1 = lua_newthread(L);
//lua_pushthread(L1);
//return lua_yield(L, args);/* ����� ������� C �������� lua_yield ����� �������, ���������� ����������� ����������������
//���� ����������, � ����� lua_resume ���� ���������� ��������� ������������.*/
//lua_insert(L, 1);//���������� ����� � ��������� �����.
//int stacksize = lua_gettop(L);
//for (int i = 1; i < stacksize; i++) {
//	lua_xmove(L, L1, 1);// ������� � L1 ��������� �������� L.
//}
//allstate.emplace(L1, L);// �������� � map.
//lua_State* L1 = luaL_newstate();
//luaL_openlibs(L1);// ������� ����. ����������.
//funs(L1);// ������ ���� �������.
//lua_pushlightuserdata(L, L);  /*��������� �����, ������� �������� ������ � ����. */
//lua_gettable(L, LUA_REGISTRYINDEX);  /* �������� ������� � �������� ����� ����� � -1 */
//char const* luaname = lua_tostring(L, -1);//��� lua �������.	
//lua_pop(L, 1);
//luastate.push_back(L1);// �������� ����� ��������� � list	
//char* name = (char*)luaname;//������ ���.
//const char* namelua = newname(name);
//	if (LUA_TUSERDATA == lua_type(L, -1)) {// �������� �����.
//		CWorld::Players[CWorld::PlayerInFocus].m_nMoney += 1;// ���� ����� 
//		CPed* ped = (CPed*)Userdata::get<CPed>(L, 1, false);
//		Stack<CPed*>::push(L1, ped);// ��������� � ���� ��������� �� ������.			
//	}
//};
//	int status = luaL_loadfile(L1, namelua);
//			CWorld::Players[CWorld::PlayerInFocus].m_nMoney += 10;// ���� ����� 
				//			lua_pushvalue(L1, 1);//����������� ����� �������, ��������� �� ������� �����.

		//if (LUA_TFUNCTION == lua_type(L1, 1)) {
//		}



//const void* value = lua_topointer(L, i);// �������� �������������� ��������� �� �����.������.
			//lua_pushinteger(L, (int)& value);  /*��������� �����, ������� �������� ������ � ����. */
			//lua_gettable(L, LUA_REGISTRYINDEX);  /* �������� ������� � �������� ����� ����� � -1 */
			//if (LUA_TSTRING == lua_type(L, -1)) {//CWorld::Players[CWorld::PlayerInFocus].m_nMoney += 10000;// ���� ����� 
			//	const char* clas = lua_tostring(L, -1);// ��� ����� �����.������ � ������� �����.
			//	lua_pop(L, 1);		//wchar_t* str = getwchat(clas);
			//	const char* st = "cped";//		CMessages::AddMessageJumpQ(str, 6000, 1);
			//	if (strcmp(clas, st) == 0) {
			//		lua_pop(L, 1);
			//	     }
			//    }


//bool keyup(unsigned int key) {
//	if (!KeyCheck::CheckWithDelay(key, 200)) { return true; }
//	else { return false; }
//};







//lua_State* L1 = luaL_newstate();
//luaL_openlibs(L1);// ������� ����. ����������.
//funs(L1);// ������ ���� �������.
//lua_pushlightuserdata(L, L);  /*��������� �����, ������� �������� ������ � ����. */
//lua_gettable(L, LUA_REGISTRYINDEX);  /* �������� ������� � �������� ����� ����� � -1 */
//char const* luaname = lua_tostring(L, -1);//��� lua �������.	
//lua_pop(L, 1);
//int stacksize = lua_gettop(L);// ���-�� ��������� �  ����.	
//stacksize++;
//for (int i = 1; i < stacksize; i++) {
//	if (LUA_TUSERDATA == lua_type(L, i)) {// �������� �����.
//		const void* value = lua_topointer(L, i);// �������� �������������� ��������� �� �����.������.
//		lua_pushinteger(L, (int)& value);  /*��������� �����, ������� �������� ������ � ����. */
//		lua_gettable(L, LUA_REGISTRYINDEX);  /* �������� ������� � �������� ����� ����� � -1 */
//		if (LUA_TSTRING == lua_type(L, -1)) {
//			CWorld::Players[CWorld::PlayerInFocus].m_nMoney += 10000;// ���� ����� 
//			const char* clas = lua_tostring(L, -1);// ��� ����� �����.������ � ������� �����.
//			lua_pop(L, 1);		//wchar_t* str = getwchat(clas);
//			const char* st = "cped";//		CMessages::AddMessageJumpQ(str, 6000, 1);
//			if (strcmp(clas, st) == 0) {
//				CWorld::Players[CWorld::PlayerInFocus].m_nMoney += 1;// ���� ����� 
//				CPed* ped = (CPed*)Userdata::get<CPed>(L, i, false);
//				Stack<CPed*>::push(L1, ped);// ��������� � ���� ��������� �� ������.
//				lua_pop(L, 1);
//			}
//		}
//	}
//};
//luastate.push_back(L1);// �������� ����� ��������� � list	
	//	if (res == LUA_YIELD) {
	//		res = lua_resume(L, L1, 0); // main
	//		lua_pop(L, 1);
	//		if (res == 0) { break; }
	//		else {res1 = lua_resume(L1, L, 0);// foo
	//		}
	//			/*L1 = lua_tothread(L, -1); lua_pop(L, 1); lua_xmove(L, L1, lua_gettop(L)); lua_remove(L1, 1); args = lua_gettop(L1);
	//			for (int i = 1; i > args; i++) { lua_pushvalue(L1, i); }
	//			args--;	res1 = lua_resume(L1, L, args);	lua_xmove(L, L1, 1);
	//		lua_sethook(L, LUAHook, LUA_MASKCOUNT, 6);*/
	//			for (mll it = allstate.begin(); it != allstate.end(); ++it) {
	//				if (L == it->second) {	L1 = it->first;
	//	if (LUA_TTHREAD == lua_type(L, -1) && LUA_TNUMBER == lua_type(L, -2)) {
	//		args = lua_tointeger(L, -2);
	//		showstack(L);
	//		CWorld::Players[CWorld::PlayerInFocus].m_nMoney += 100;
	//		this_thread::sleep_for(chrono::milliseconds(5200));
	//	}
	//			lua_gettop(L1);//CWorld::Players[CWorld::PlayerInFocus].m_nMoney += 1;
	//		if (LUA_TFUNCTION == lua_type(L1, -2)) {
	//			CWorld::Players[CWorld::PlayerInFocus].m_nMoney += 1;
	//					for (int i = 1; i < args; i++) { lua_insert(L1, i);	//	lua_pushvalue(L, i);
	//					}//���������� ����� � ��������� �����.
	//					args--;/* ���������.*/  //lua_sethook(L1, LUAHook1, LUA_MASKCOUNT, 1);
	//					res1 = lua_resume(L1, L, args); //lua_settop(L1, args++);;	
	//					showstack1(L1);
	//					args++;					} 					} 
	//				}
	//        }
	//   if ( res1 == LUA_YIELD ||  res1 == LUA_OK ) {
	//	   CWorld::Players[CWorld::PlayerInFocus].m_nMoney += 1;	
	//	   lua_sethook(L, LUAHook, LUA_MASKCOUNT, 10);	
	//	   lua_getglobal(L, "main");
	//   res = lua_resume(L, L1, 0); //int li =  //  
	//   lua_sethook(L, LUAHook, LUA_MASKCOUNT, 0);
	//   showstack(L); lua_pop(L1, 1); 
	//   this_thread::sleep_for(chrono::milliseconds(100));
	//   break;
	//   
	// lua_pop(L1, 1); 
	//   CWorld::Players[CWorld::PlayerInFocus].m_nMoney += 1;// size =  args-size;   
	//   lua_pop(L1, 1); if (LUA_TFUNCTION == lua_type(L1, -1)) {
	//   int size;	
	//   size = lua_gettop(L1); 
	//   if (size > args) {
	//	 if (size >5)		 {
	//	 size = size- args;  
	//	 size--;
	//   lua_pop(L1, size);
	//	 }
	//   }
	//  
	//   args++;
	//   CWorld::Players[CWorld::PlayerInFocus].m_nMoney += args;
	//    this_thread::sleep_for(chrono::milliseconds(5200));
	//	   size++;
	//	   size = size - args;
	//	   CWorld::Players[CWorld::PlayerInFocus].m_nMoney += size;
	//   cout << "lkj " << endl;
	//   lua_settop(L1, args);
	//	   for (int i = 1; i > size; i++) {
	//		    if (i > 2 )  {
	//		  
	//   }
	//	   this_thread::sleep_for(chrono::milliseconds(5)); 
	//	   size =  args-size;
	//	   CWorld::Players[CWorld::PlayerInFocus].m_nMoney += size;// ���� ����� size++;
	//   args--;
	//	   lua_pop(L1, 1);
	//.	   this_thread::sleep_for(chrono::milliseconds(1));
	//   }
	//	}
	//}

	//lua_pushinteger(L, (int)& value);  /*��������� �����, ������� �������� ������ � ����. */
	//lua_gettable(L, LUA_REGISTRYINDEX);  /* �������� ������� � �������� ����� ����� � -1 */
	//if (LUA_TSTRING == lua_type(L, -1)) {
	//	const char* clas = lua_tostring(L, -1);// ��� ����� �����.������ � ������� �����.
	//	lua_pop(L, 1);	const char* st = "cped";//	
	//	if (strcmp(clas, st) == 0) {
		//CPed* p = (CPed*)Userdata::get<CPed>(L, 1, false);// �������� ��������� �� ������.
		//if (LUA_TSTRING == lua_type(L, -1)) {
		//	string str  = lua_tostring(L, -1);
		//	str.erase(0, 10);
		//	
		//	const char* x = strdup(str.c_str());
		//	wchar_t* s1 = getwchat(x);
		//	CMessages::AddMessageJumpQ(s1, 3000, 0);  
		//		for (auto ped : CPools::ms_pPedPool) {
		//			int p = (int)ped;
		//			std::string sped = to_string(p);
		//			if (str == sped) {
		//				Stack<bool>::push(L, true); Stack<CPed*>::push(L, ped);// ��������� � ���� � �������� �� ����� �����
		//				return 2;
		//			}
		//		}
		//		CPed* p2 = nullptr;   Stack<bool>::push(L, false);
		//		Stack<CPed*>::push(L, p2);// ��������� � ���� � �������� �� ����� �����
		//		return 2;
		//		}

//��������� ������������ �������� � ������ ���������� �����������
//���������
//
//03CC: add_stuck_car_check[����� ����������] ����������[float] �����[int]
//��������
//
//����� ����������
//����� ������������� ��������
//��������
//����������� ����������, ������� ���������� ������ �������� � ��������
//���
//����������������� ������� � ��
//������ ������
//
//ADD_STUCK_CAR_CHECK
//���� ��� �������� ��������� ���������� ������������� �������� ������ � ��������������� ����������� � ����������� �������, ����� ���������, �� ������� �� ��.���� ��������� ���������, ��� �� ������ �� ����� ������� ������������� �����������.������������ �������� ���������� ��� ����������, ���� ��� �� ��������� ����������� ����������, ������������� � �������� ������� ��������� � ������� ���������� ������� �������, �������������� � �������� �������� ���������.���� ������������ �������� ����������, ��� ��������� �� ������� ���������� �����������.������ ���������� ����������� ����� �������� �� 6 ����� ����������.
//
//