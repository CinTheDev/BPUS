Image empty(0, 0, 0);
Image test("test2.jpg");
Image alphatest("test3.png");


//#include "../BPUS_Server/BPUS_Common.h"
//#include <unordered_map>

Obj::Empty* obj;
Obj::Baseobject* obj2;

class BPUS_Game{
private:
	// Networking shall be disabled for now, but the infrastructure remains
	// It will be enabled in the future

	/*std::unordered_map<uint32_t, sPlayerDesc> mapObjects;
	uint32_t nPlayerID = 0;
	sPlayerDesc descPlayer;

	bool bWaitingForConnection = true;*/

public:
	BPUS_Game() {
		// Camera
		camera = new Obj::Camera(Vector2(0, 0), &empty, 1.f);
		Obj_M::create(camera);

		//obj = new Obj::Empty(h_Vector2(0, 0), &test, 1.f);
		//obj->z = 1;
		obj2 = new Obj::Baseobject(Vector2(500, 0), &alphatest, 1.f);
		obj2->z = 0;
		//Obj_M::create(obj);
		Obj_M::create(obj2);

		//Connect("127.0.0.1", 60000);
	}

	void update(Input* input, float dt) {
		//listenMessages();

		Obj_M::objects_update(input, dt);

		// Render
		render();

		//sendData();
	}

	/*void sendData() {
		olc::net::message<GameMsg> msg;
		msg.header.id = GameMsg::Game_UpdatePlayer;
		msg << mapObjects[nPlayerID];
		Send(msg);
	}

	void listenMessages() {
		if (IsConnected()) {
			while (!Incoming().empty()) {
				auto msg = Incoming().pop_front().msg;

				switch (msg.header.id)
				{
				case(GameMsg::Client_Accpepted): {
					olc::net::message<GameMsg> msg;
					msg.header.id = GameMsg::ClientRegisterWithServer;
					descPlayer.pos = Vector2(3.f, 3.f);
					descPlayer.posX = 3.f;
					descPlayer.posY = 3.f;
					msg << descPlayer;
					Send(msg);
					break;
				}

				case(GameMsg::Client_AssignID): {
					// Server is assigning us OUR id
					msg >> nPlayerID;
					break;
				}

				case(GameMsg::Game_AddPlayer): {
					sPlayerDesc desc;
					msg >> desc;
					mapObjects.insert_or_assign(desc.nUniqueID, desc);

					if (desc.nUniqueID == nPlayerID) {
						// How we exist in game world
						bWaitingForConnection = false;
					}
					break;
				}

				case (GameMsg::Game_RemovePlayer): {
					uint32_t nRemovalID = 0;
					msg >> nRemovalID;
					mapObjects.erase(nRemovalID);
					break;
				}

				case(GameMsg::Game_UpdatePlayer): {
					sPlayerDesc desc;
					msg >> desc;
					mapObjects.insert_or_assign(desc.nUniqueID, desc);
					break;
				}

				}
			}
		}
	}*/
};
