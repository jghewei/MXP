#include "DanOS.h"
#include "PortMob.h"
#include "Web.h"

void WebPortGet(Webs *wp)
{
    int inst = 0, num = PortMob::GetInstNum();
    char *pid = websGetVar(wp, "id", "");
    if (strlen(pid) > 0)
    {
        inst = atoi(pid);
        num = inst + 1;
    }

    CJson webs;
    for (; inst < num; inst++)
    {
        PortMob *pMob = PortMob::GetInst(inst);
        if (pMob == NULL) continue;

        webs.AddEntry();
        webs.AddValue("Key", pMob->key.name());
        webs.AddValue("AliasName", pMob->AliasName.Get());
        webs.AddValue("PortMode", pMob->PortMode.Get());
        webs.AddValue("AdminStatus", pMob->AdminStatus.Get());
        webs.AddValue("OperStatus", pMob->OperStatus.Get());
        webs.AddValue("LoopbackType", pMob->LoopbackType.Get());

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

void WebPortSet(Webs *wp)
{
    int inst = 0;
    char *ptr = NULL;

    ptr = websGetVar(wp, "id", "");
    if (strlen(ptr) > 0)
    {
        inst = atoi(ptr);
    }

    PortMob *pMob = PortMob::GetInst(inst);
    if (pMob == NULL) goto error_exit;

    ptr = websGetVar(wp, "AliasName", "");
    if (strlen(ptr) > 0)
    {
        pMob->AliasName.Set(ptr);
    }

    ptr = websGetVar(wp, "PortMode", "");
    if (strlen(ptr) > 0)
    {
        pMob->PortMode.Set(atoi(ptr));
    }

    ptr = websGetVar(wp, "AdminStatus", "");
    if (strlen(ptr) > 0)
    {
        pMob->AdminStatus.Set(atoi(ptr));
    }

    ptr = websGetVar(wp, "LoopbackType", "");
    if (strlen(ptr) > 0)
    {
        pMob->LoopbackType.Set(atoi(ptr));
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

void WebPortAction(Webs *wp)
{
    if (isWebPostMethod(wp))
        WebPortSet(wp);
    else
        WebPortGet(wp);
}

