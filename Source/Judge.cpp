#include "Judge.h"
#include "Player.h"
#include <iostream>
#include "Effect.h"

#include "Judge.h"
#include "WinMain.h"
#include "Player.h"
#include "Player_sd.h"
#include <iostream>
#include "Effect.h"
#include "Wall.h"


class JudgeRect
{
	float left, top, right, bottom;

public:
	JudgeRect(float l, float t, float r, float b)
	{
		left = l;
		top = t;
		right = r;
		bottom = b;
	}
	JudgeRect(const VECTOR2& position, const VECTOR2& hSize)
		:JudgeRect(position.x - hSize.x, position.y - hSize.y,
			position.x + hSize.x, position.y + hSize.y) {}

	bool isHit(const JudgeRect& other)
	{
		if (right < other.left) return false;
		if (left > other.right) return false;
		if (bottom < other.top) return false;
		if (top > other.bottom) return false;
		return true;
	}

};

// プロトタイプ宣言
void judgeSub(OBJ2DManager& manager1, OBJ2DManager& manager2);
//void judgePvP(OBJ2DManager& manager1, OBJ2DManager& manager2);
void judgePvP(OBJ2DManager& manager);


JudgeRect screenRect = {
	64, 64, SCREEN_WIDTH - 64, SCREEN_HEIGHT - 64,
};

void judge()
{
	//プレイヤーVS壁
	judgeSub(PlayerManager::getInstance(), WallManager::getInstance());

	//// プレイヤー2VS壁
	//judgeSub(PlayerManager_sd::getInstance(), WallManager::getInstance());

	// プレイヤーVSプレイヤー2
	judgePvP(PlayerManager::getInstance());


}


void judgeSub(OBJ2DManager& manager1, OBJ2DManager& manager2)
{
	for (auto& item1 : manager1)
	{
		if (!item1.mover) continue;

		JudgeRect rect1(item1.position, item1.hSize);
		// rect1があたり判定エリア外のとき、次のitem1へ
		if (!screenRect.isHit(rect1)) continue;

		for (auto& item2 : manager2)
		{
			if (!item2.mover) continue;
			if ((item1.judge & item2.judge) == 0) continue;

			JudgeRect rect2(item2.position, item2.hSize);
			// rect2があたり判定エリア外のとき、次のitem2へ
			if (!screenRect.isHit(rect2)) continue;

			if (rect1.isHit(rect2))
			{
				// 押し戻す方向を計算
				VECTOR2 pushDir = item1.position - item2.position;
				float lenSq = vec2LengthSq(pushDir);

				if (lenSq > 0.01f)
				{
					VECTOR2 normal = vec2Normalize(pushDir);
					float overlapX = (item1.hSize.x + item2.hSize.x) - std::abs(item1.position.x - item2.position.x);
					float overlapY = (item1.hSize.y + item2.hSize.y) - std::abs(item1.position.y - item2.position.y);

					//overlapX = 1.0;
					//overlapY = 1.0;

					// より重なりが大きい方向に補正（軸ごとにずらす）
					if (overlapX < overlapY)
						item1.position.x += (item1.position.x < item2.position.x) ? -overlapX - 0.1f : overlapX + 0.1f;//0.1f,ここで壁の隙間を増やす
					else
						item1.position.y += (item1.position.y < item2.position.y) ? -overlapY - 0.1f : overlapY + 0.1f;
				}
				direction_reset(&item1);

				break;
			}
		}
	}
}

void judgePvP(OBJ2DManager& manager)
{
	for (auto& item1 : manager)
	{
		if (!item1.mover) continue;

		JudgeRect rect1(item1.position, item1.hSize);
		// rect1があたり判定エリア外のとき、次のitem1へ
		if (!screenRect.isHit(rect1)) continue;

		for (auto& item2 : manager)
		{
			if (!item2.mover) continue;
			if (&item1 == &item2) continue;
			if ((item1.judge & item2.judge) == 0) continue;

			JudgeRect rect2(item2.position, item2.hSize);
			// rect2があたり判定エリア外のとき、次のitem2へ
			if (!screenRect.isHit(rect2)) continue;

			if (rect1.isHit(rect2))
			{
				// 押し戻す方向を計算
				VECTOR2 pushDir = item1.position - item2.position;
				float lenSq = vec2LengthSq(pushDir);

				if (lenSq > 0.01f)
				{
					VECTOR2 normal = vec2Normalize(pushDir);
					float overlapX = (item1.hSize.x + item2.hSize.x) - std::abs(item1.position.x - item2.position.x);
					float overlapY = (item1.hSize.y + item2.hSize.y) - std::abs(item1.position.y - item2.position.y);

					//overlapX = 1.0;
					//overlapY = 1.0;

					// より重なりが大きい方向に補正（軸ごとにずらす）
					if (overlapX < overlapY)
						item1.position.x += (item1.position.x < item2.position.x) ? -overlapX - 0.1f : overlapX + 0.1f;//0.1f,ここで壁の隙間を増やす
					else
						item1.position.y += (item1.position.y < item2.position.y) ? -overlapY - 0.1f : overlapY + 0.1f;
				}

				direction_reset(&item1);
				direction_reset(&item2);

				break; // 一つに対してのみ反応（必要ならcontinueに変更）
			}
		}
	}
}

