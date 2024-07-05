#include "Graphics/Graphics.h"
#include "SceneGame.h"
#include "Camera.h"
#include "EnemyManager.h"
#include "EnemySlime.h"
#include "EffectManager.h"
#include "Input/Input.h"
#include "StageManager.h"
#include "StageMain.h"
#include "StageMoveFloor.h"
// 初期化
void SceneGame::Initialize()
{

	StageManager& stageManager = StageManager::Instance();
	StageMain* stageMain = new StageMain();
	stageManager.Register(stageMain);

	StageMoveFloor* stageMoveFloor = new StageMoveFloor();
	stageMoveFloor->SetStartPoint(DirectX::XMFLOAT3(0, 1, 3));
	stageMoveFloor->SetGoalPoint(DirectX::XMFLOAT3(10, 2, 3));
	stageMoveFloor->SetTorque(DirectX::XMFLOAT3(0, 1.0f, 0));
	stageManager.Register(stageMoveFloor);


	player = new Player();

	gauge = new Sprite();

	cameraController = new CameraController;

	//カメラ初期設定
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 10, -10),// カメラの視点（位置）
		DirectX::XMFLOAT3(0, 0, 0),	// カメラの注視点（ターゲット）
		DirectX::XMFLOAT3(0, 1, 0)		// カメラの上方向

		);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),	// 視野角
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),	// 画面縦横比率
		0.1f,	// カメラが映し出すの最近距離
		1000.0f	// カメラが映し出すの最遠距離
	);
	//エネミーを配置
	for (int i = 0; i < 2; ++i)
	{
	EnemySlime* Slime = new EnemySlime();
	Slime->SetPosition(DirectX::XMFLOAT3(i * 2.0f, 0, 5.0f));
	EnemyManager::Instance().Register(Slime);

	}
	sprite = new Sprite("Data/Sprite/LoadingIcon.png");
}

// 終了化
void SceneGame::Finalize()
{
	StageManager::Instance().Clear();
	if (player != nullptr)
	{
		delete player;
		player = nullptr;
	}
	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}
	if (gauge != nullptr)
	{
		delete gauge;
		gauge = nullptr;
	}

	EnemyManager::Instance().Clear();

}

// 更新処理
void SceneGame::Update(float elapsedTime)
{
	DirectX::XMFLOAT3 target = player->GetPosition();
	target.y += 0.5f;
	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);

	StageManager::Instance().Update(elapsedTime);

	player->Update(elapsedTime);

	EnemyManager::Instance().Update(elapsedTime);

	EffectManager::Instance().Update(elapsedTime);
}

// 描画処理
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// 画面クリア＆レンダーターゲット設定
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0～1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// 描画処理
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ライト方向（下方向）

	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();


	//// ビュー行列
	//{
	//	DirectX::XMFLOAT3 eye = { 0, 10, -10 };	// カメラの視点（位置）
	//	DirectX::XMFLOAT3 focus = { 0, 0, 0 };	// カメラの注視点（ターゲット）
	//	DirectX::XMFLOAT3 up = { 0, 1, 0 };		// カメラの上方向

	//	DirectX::XMVECTOR Eye = DirectX::XMLoadFloat3(&eye);
	//	DirectX::XMVECTOR Focus = DirectX::XMLoadFloat3(&focus);
	//	DirectX::XMVECTOR Up = DirectX::XMLoadFloat3(&up);
	//	DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(Eye, Focus, Up);
	//	DirectX::XMStoreFloat4x4(&rc.view, View);
	//}
	//// プロジェクション行列
	//{
	//	float fovY = DirectX::XMConvertToRadians(45);	// 視野角
	//	float aspectRatio = graphics.GetScreenWidth() / graphics.GetScreenHeight();	// 画面縦横比率
	//	float nearZ = 0.1f;	// カメラが映し出すの最近距離
	//	float farZ = 1000.0f;	// カメラが映し出すの最遠距離
	//	DirectX::XMMATRIX Projection = DirectX::XMMatrixPerspectiveFovLH(fovY, aspectRatio, nearZ, farZ);
	//	DirectX::XMStoreFloat4x4(&rc.projection, Projection);
	//}

	// 3Dモデル描画
	{
		//Graphics& graphics1 = Graphics::Instance();
		//ID3D11DeviceContext* dc1 = graphics1.GetDeviceContext();
		//ID3D11RenderTargetView* rtv1 = graphics1.GetRenderTargetView();
		//ID3D11DepthStencilView* dsv1 = graphics1.GetDepthStencilView();

		//// 画面クリア＆レンダーターゲット設定
		//FLOAT color1[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0～1.0)
		//dc->ClearRenderTargetView(rtv1, color1);
		//dc->ClearDepthStencilView(dsv1, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		//dc->OMSetRenderTargets(1, &rtv1, dsv1);

		//// 2Dスプライト描画
		//{
		//	float screenWidth = static_cast<float>(graphics1.GetScreenWidth());
		//	float screenHeight = static_cast<float>(graphics1.GetScreenHeight());
		//	float textureWidth = static_cast<float>(sprite->GetTextureWidth());
		//	float textureHeight = static_cast<float>(sprite->GetTextureHeight());
		//	 タイトルスプライト描画
		//	sprite->Render(dc,
		//		0.1, 0.1, screenWidth, screenHeight,
		//		0.1, 0.1, textureWidth, textureHeight,
		//		0.1,
		//		1, 1, 1, 1);
		//}
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);

		StageManager::Instance().Render(dc, shader);

		player->Render(dc, shader);
		EnemyManager::Instance().Render(dc, shader);
		shader->End(dc);

	}
	{
		EffectManager::Instance().Render(rc.view,rc.projection);
	}

	// 3Dデバッグ描画
	{
		//プレイヤーデバッグプリミティブ描画
		player->DrawDebugPrimitive();

		EnemyManager::Instance().DrawDebugPrimitive();

		// ラインレンダラ描画実行
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// デバッグレンダラ描画実行
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
	}

	// 2Dスプライト描画
	{
		RenderEnemyGauge(dc, rc.view, rc.projection);

		
	}

	// 2DデバッグGUI描画
	{
		//プレイヤーデバッグ
		player->DrawDebugGUI();
	}
}

void SceneGame::RenderEnemyGauge(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	dc->RSGetViewports(&numViewports, &viewport);

	// 変換行列
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	// 全ての敵の頭上にHPゲージを表示
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyCount = enemyManager.GetEnemyCount();

	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);

		// エネミーの頭上のワールド座標
		DirectX::XMFLOAT3 worldPosition = enemy->GetPosition();
		worldPosition.y += enemy->GetHeight();

		DirectX::XMVECTOR WorldPosition = DirectX::XMLoadFloat3(&worldPosition);

		// ワールド座標からスクリーン座標へ変換
		DirectX::XMVECTOR ScreenPosition = DirectX::XMVector3Project(
			WorldPosition,
			viewport.TopLeftX,
			viewport.TopLeftY,
			viewport.Width,
			viewport.Height,
			viewport.MinDepth,
			viewport.MaxDepth,
			Projection,
			View,
			World
		);

		// スクリーン座標
		DirectX::XMFLOAT2 screenPosition;
		DirectX::XMStoreFloat2(&screenPosition, ScreenPosition);

		// ゲージの長さ
		const float gaugeWidth = 30.0f;
		const float gaugeHeight = 5.0f;

		float healthRate = enemy->GetHealth() / static_cast<float>(enemy->GetMaxHealth());

		//　ゲージ描画
		gauge->Render(dc,
			screenPosition.x - gaugeWidth * 0.5f,
			screenPosition.y - gaugeHeight,
			gaugeWidth * healthRate,
			gaugeHeight,
			0, 0,
			static_cast<float>(gauge->GetTextureWidth()),
			static_cast<float>(gauge->GetTextureHeight()),
			0.0f,
			1.0f, 0.0f, 0.0f, 1.0f
		);
	}
	Mouse& mouse = Input::Instance().GetMouse();
	if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
	{
		// マウスカーソル座標を取得
		DirectX::XMFLOAT3 screenPosition;
		screenPosition.x = static_cast<float>(mouse.GetPositionX());
		screenPosition.y = static_cast<float>(mouse.GetPositionY());

		DirectX::XMVECTOR ScreenPosition, WorldPosition;

		// レイの始点を算出
		screenPosition.z = 0.0f;
		ScreenPosition = DirectX::XMLoadFloat3(&screenPosition);
		WorldPosition = DirectX::XMVector3Unproject(
			ScreenPosition,
			viewport.TopLeftX,
			viewport.TopLeftY,
			viewport.Width,
			viewport.Height,
			viewport.MinDepth,
			viewport.MaxDepth,
			Projection,
			View,
			World
		);
		DirectX::XMFLOAT3 rayStart;
		DirectX::XMStoreFloat3(&rayStart, WorldPosition);

		// レイの終点を算出
		screenPosition.z = 1.0f;
		ScreenPosition = DirectX::XMLoadFloat3(&screenPosition);
		WorldPosition = DirectX::XMVector3Unproject(
			ScreenPosition,
			viewport.TopLeftX,
			viewport.TopLeftY,
			viewport.Width,
			viewport.Height,
			viewport.MinDepth,
			viewport.MaxDepth,
			Projection,
			View,
			World
		);
		DirectX::XMFLOAT3 rayEnd;
		DirectX::XMStoreFloat3(&rayEnd, WorldPosition);

		// レイキャスト
		HitResult hit;
		if (StageManager::Instance().RayCast(rayStart, rayEnd, hit))
		{
			// 敵を配置
			EnemySlime* slime = new EnemySlime();
			slime->SetPosition(hit.position);
			EnemyManager::Instance().Register(slime);
		}
	}
}
