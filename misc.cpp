#include "misc.h"





void misc()
{
	if (ImGui::BeginTabItem(u8"ÔÓÏî"))
	{
		ImGui::Checkbox(u8"Á¬Ìø", &bhop);

		ImGui::Spacing();
		ImGui::Checkbox(u8"Ñ¹Ç¹", &rcs);
		ImGui::EndTabItem();
	}
}


DWORD WINAPI RcsThread(LPVOID lp)
{
	while (true)
	{
		if (rcs && !GetAsyncKeyState(VK_RBUTTON))
		{
			Vec3 punchAngle = *aimRecoilPunch * (rcs_amount * 2);
			if (*iShotsFired > 1 && GetAsyncKeyState(VK_LBUTTON))
			{
				Vec3 newAngle = *viewAngles + oPunch - punchAngle;
				newAngle.normailze();

				*viewAngles = newAngle;
			}
			oPunch = punchAngle;
		}
	}
}

DWORD WINAPI BhopThread(LPVOID lp)
{
	DWORD gameMoudle = (DWORD)GetModuleHandle("client.dll");
	DWORD localplayer = *(DWORD*)(gameMoudle + dwLocalPlayer);
	while (localplayer == NULL)
	{
		localplayer = *(DWORD*)(gameMoudle + dwLocalPlayer);
	}
	while (true)
	{
		if (bhop)
		{
			DWORD flag = *(BYTE*)(localplayer + m_fFlags);
			if (GetAsyncKeyState(VK_SPACE) && flag & (1 << 0))
			{
				*(DWORD*)(gameMoudle + dwForceJump) = 6;
			}
		}
	}
}