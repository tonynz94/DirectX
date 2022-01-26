#include "pch.h"
#include "Game.h"
#include "Engine.h"

shared_ptr<Mesh> mesh = make_shared<Mesh>();
shared_ptr<Shader> shader = make_shared<Shader>();
//����� CPU�� �ϴ°�
//���� �ﰢ���� ��� ������ ������ �ؼ� �̵��Ҷ� ���⼭ +,-�� ���ָ� CPU�� �� ���� �� ó���ϰ� ��
//�̷��͵��� Shader(GPU)�� �ϵ��� �ؾ� ��)
void Game::Init(const WindowInfo& info)
{
	GEngine->Init(info);

	//����  ���� -1 ���� ������ +1 
	//���ϾƷ��� -1 ���� ���� +1
	//(�� ������ �ϳ��� ����)
	//�̸� �̵��ϰų� ���� ĥ�ϴ°Ŵ� GPU���� ���ֱ� ������ ���⼭ ���ָ� �ȵ�.
	vector<Vertex> vec(3);
	vec[0].pos = Vec3(0.f, 0.5f, 0.5f); //x,y 3������ ����
	vec[0].color = Vec4(1.f, 0.f, 0.f, 1.f);
	vec[1].pos = Vec3(0.5f, -0.5f, 0.5f);
	vec[1].color = Vec4(0.f, 1.0f, 0.f, 1.f);
	vec[2].pos = Vec3(-0.5f, -0.5f, 0.5f);
	vec[2].color = Vec4(0.f, 0.f, 1.f, 1.f);
	mesh->Init(vec);

	shader->Init(L"..\\Resources\\Shader\\default.hlsli");

	//�ϳ��������� ��ٷ� �ִ� ��
	GEngine->GetCmdQueue()->WaitSync();

}

//�� �����Ӹ��� ����.
void Game::Update()
{
	GEngine->RenderBegin();

	shader->Update();

	{
		Transform t;
		t.offset = Vec4(0.75f, 0.f, 0.f, 0.f);
		mesh->SetTransform(t);

		mesh->Render();
	}

	{
		Transform t;
		t.offset = Vec4(0.f, 0.75f, 0.f, 0.f);
		mesh->SetTransform(t);

		mesh->Render();
	}

	GEngine->RenderEnd();
}
