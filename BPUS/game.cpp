Image empty(0, 0, 0);
Image test("test2.jpg");
Image alphatest("test3.png");

#include "Object_behavior.cpp"
#include "../BPUS_Server/BPUS_Common.h"
#include <unordered_map>

Object* obj;
Object* obj2;

class BPUS_Game : public olc::net::client_interface<GameMsg>{
private:
	std::unordered_map<uint32_t, sPlayerDesc> mapObjects;
	uint32_t nPlayerID = 0;
	sPlayerDesc descPlayer;

	bool bWaitingForConnection = true;

public:
	BPUS_Game() {
		// Camera
		camera = new Obj::Camera(Vector2(0, 0), &empty, 1.f);
		Obj_M::create(camera);

		//obj = new Obj::Empty(Vector2(0, 0), &test, 1.f);
		//obj->z = 1;
		obj2 = new Obj::Baseobject(Vector2(500, 0), &alphatest, 1.f);
		obj2->z = 0;
		//Obj_M::create(obj);
		Obj_M::create(obj2);

		Connect("127.0.0.1", 60000);
	}

	void update(Input* input, float dt) {

		if (IsConnected()) {
			while (!Incoming().empty()) {
				auto msg = Incoming().pop_front().msg;

				switch (msg.header.id)
				{
				case(GameMsg::Client_Accpepted): {
					olc::net::message<GameMsg> msg;
					msg.header.id = GameMsg::ClientRegisterWithServer;
					//descPlayer.pos = Vector2(3.f, 3.f);
					//descPlayer.pos = { 3.f, 3.f };
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

		Obj_M::objects_update(input, dt);

		// Render
		draw_background(0x000011);

		draw_rect(Vector2(300, 100), Vector2(100, 300), 0xAA0055);
		draw_tri(Vector2(100, 100), Vector2(200, 150), Vector2(180, 250), 0x00AA55);
		draw_cir(Vector2(300, 100), 50, 0x55AA00);
		draw_text(Vector2(500, 200), "The Quick Brown Fox Jumps Over The Lazy Dog\nTHE QUICK BROWN FOX JUMPS OVER THE LAZY DOG\nthe quick brown fox jumps over the lazy dog", 2.5f);

		// Debug
		Vector2 v(3, 4);
		Vector2 v_norm = v;
		v_norm.normalize();
		Vector2 v_fast = v.normalized();
		std::string s = v.str() + v_norm.str() + v_fast.str();
		draw_text(camera->position + Vector2(5, 20), s.c_str(), 2.f);

		draw_objects();

		olc::net::message<GameMsg> msg;
		msg.header.id = GameMsg::Game_UpdatePlayer;
		msg << mapObjects[nPlayerID];
		Send(msg);
	}
};
