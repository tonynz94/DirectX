#include "pch.h"
#include "Game.h"
#include "Engine.h"

shared_ptr<Mesh> mesh = make_shared<Mesh>();
shared_ptr<Shader> shader = make_shared<Shader>();
//여기는 CPU가 하는것
//만약 삼각형의 모든 정점에 덧셈을 해서 이동할때 여기서 +,-를 해주면 CPU가 이 일을 다 처리하게 됨
//이런것들을 Shader(GPU)가 하도록 해야 됨)
void Game::Init(const WindowInfo& info)
{
	GEngine->Init(info);

	//제일  왼쪽 -1 제일 오른쪽 +1 
	//제일아래쪽 -1 제일 위쪽 +1
	//(모델 정보는 하나만 존재)
	//이를 이동하거나 색을 칠하는거는 GPU에서 해주기 떄문에 여기서 해주면 안됨.


	//============인덱스 버퍼를 사용하지 않으면 여섯개의 정점정보를 알려줘야함==================
	//vector<Vertex> vec(6);
	//vec[0].pos = Vec3(-0.5f, 0.5f, 0.5f);  //x,y 3번쨰는 무시
	//vec[0].color = Vec4(1.f, 0.f, 0.f, 1.f);	//R
	//vec[1].pos = Vec3(0.5f, 0.5f, 0.5f);
	//vec[1].color = Vec4(0.f, 1.0f, 0.f, 1.f);	//G
	//vec[2].pos = Vec3(0.5f, -0.5f, 0.5f);
	//vec[2].color = Vec4(0.f, 0.f, 1.f, 1.f);	//B

	//vec[3].pos = Vec3(0.5f, -0.5f, 0.5f); //x,y 3번쨰는 무시
	//vec[3].color = Vec4(1.f, 0.f, 0.f, 1.f);	//R
	//vec[4].pos = Vec3(-0.5f, -0.5f, 0.5f);
	//vec[4].color = Vec4(0.f, 1.0f, 0.f, 1.f);	//G
	//vec[5].pos = Vec3(-0.5f, 0.5f, 0.5f);
	//vec[5].color = Vec4(0.f, 0.f, 1.f, 1.f);	//B

	//mesh->Init(vec);

	//============버텍스 버퍼 + 인덱스 버퍼 사용시 ==================
	vector<Vertex> vec(4);
	vec[0].pos = Vec3(-0.5f, 0.5f, 0.5f);
	vec[0].color = Vec4(1.f, 0.f, 0.f, 1.f);
	vec[1].pos = Vec3(0.5f, 0.5f, 0.5f);
	vec[1].color = Vec4(0.f, 1.f, 0.f, 1.f);
	vec[2].pos = Vec3(0.5f, -0.5f, 0.5f);
	vec[2].color = Vec4(0.f, 0.f, 1.f, 1.f);
	vec[3].pos = Vec3(-0.5f, -0.5f, 0.5f);
	vec[3].color = Vec4(0.f, 1.f, 0.f, 1.f);

	vector<uint32> indexVec;
	{
		indexVec.push_back(0);
		indexVec.push_back(1);
		indexVec.push_back(2);
	}
	{
		indexVec.push_back(0);
		indexVec.push_back(2);
		indexVec.push_back(3);
	}

	mesh->Init(vec, indexVec);
	

	shader->Init(L"..\\Resources\\Shader\\default.hlsli");

	//일끝날때까지 기다려 주는 것
	GEngine->GetCmdQueue()->WaitSync();

}

//매 프레임마다 실행.
void Game::Update()
{
	GEngine->RenderBegin();

	shader->Update();

	{
		Transform t;
		t.offset = Vec4(0.0f, 0.f, 0.f, 0.f);
		mesh->SetTransform(t);

		mesh->Render();
	}

	/*{
		Transform t;
		t.offset = Vec4(0.f, 0.75f, 0.f, 0.f);
		mesh->SetTransform(t);

		mesh->Render();
	}*/

	GEngine->RenderEnd();
}
