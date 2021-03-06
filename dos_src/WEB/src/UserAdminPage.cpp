#include "DanOS.h"
#include "UserAdminMob.h"
#include "Web.h"

void WebUserAdminGet(Webs *wp)
{
    int inst = 0, num = UserAdminMob::GetInstNum();
    char *pid = websGetVar(wp, "id", "");
    if (strlen(pid) > 0)
    {
        inst = atoi(pid);
        num = inst + 1;
    }

    CJson webs;
    for (; inst < num; inst++)
    {
        UserAdminMob *pMob = UserAdminMob::GetInst(inst);
        if (pMob == NULL) continue;

        webs.AddEntry();
        webs.AddValue("Key", pMob->key.name());
        webs.AddValue("UserName", pMob->UserName.Get());
        webs.AddValue("Password", pMob->Password.Get());
        webs.AddValue("Privilege", pMob->Privilege.Get());

        webs.AddValue("id", to_string(inst));
        webs.EndEntry();
    }
    webs.End();

    websSetStatus(wp, 200);
    websWriteHeaders(wp, webs.GetLength(), 0);
    websWriteEndHeaders(wp);
    websWriteBlock(wp, (char*)webs.GetBuffer(), webs.GetLength());
    websDone(wp);
}

void WebUserAdminSet(Webs *wp)
{
    int inst = 0;
    char *ptr = NULL;

    ptr = websGetVar(wp, "id", "");
    if (strlen(ptr) > 0)
    {
        inst = atoi(ptr);
    }

    UserAdminMob *pMob = UserAdminMob::GetInst(inst);
    if (pMob == NULL) goto error_exit;

    ptr = websGetVar(wp, "UserName", "");
    if (strlen(ptr) > 0)
    {
        pMob->UserName.Set(ptr);
    }

    ptr = websGetVar(wp, "Password", "");
    if (strlen(ptr) > 0)
    {
        pMob->Password.Set(ptr);
    }

    ptr = websGetVar(wp, "Privilege", "");
    if (strlen(ptr) > 0)
    {
        pMob->Privilege.Set(atoi(ptr));
    }

    pMob->Save();

error_exit:
    std::string resp = WebFormResponse(true, "command complete");

    websSetStatus(wp, 200);
    websWriteHeaders(wp, resp.size(), 0);
    websWriteEndHeaders(wp);
    websWrite(wp, (char*)resp.c_str());
    websDone(wp);
}

void WebUserAdminAction(Webs *wp)
{
    if (isWebPostMethod(wp))
        WebUserAdminSet(wp);
    else
        WebUserAdminGet(wp);
}

