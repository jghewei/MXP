#include "DanOS.h"
#include "CardMob.h"
#include "Web.h"

void WebCardGet(Webs *wp)
{
    int inst = 0, num = CardMob::GetInstNum();
    char *pid = websGetVar(wp, "id", "");
    if (strlen(pid) > 0)
    {
        inst = atoi(pid);
        num = inst + 1;
    }

    CJson webs;
    for (; inst < num; inst++)
    {
        CardMob *pMob = CardMob::GetInst(inst);
        if (pMob == NULL) continue;

        webs.AddEntry();
        webs.AddValue("Key", pMob->key.name());
        webs.AddValue("BoardTemp", pMob->BoardTemp.Get());
        webs.AddValue("LowTempThd", pMob->LowTempThd.Get());
        webs.AddValue("HighTempThd", pMob->HighTempThd.Get());
        webs.AddValue("OperStatus", pMob->OperStatus.Get());

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

void WebCardSet(Webs *wp)
{
    int inst = 0;
    char *ptr = NULL;

    ptr = websGetVar(wp, "id", "");
    if (strlen(ptr) > 0)
    {
        inst = atoi(ptr);
    }

    CardMob *pMob = CardMob::GetInst(inst);
    if (pMob == NULL) goto error_exit;

    ptr = websGetVar(wp, "LowTempThd", "");
    if (strlen(ptr) > 0)
    {
        pMob->LowTempThd.Set(atoi(ptr));
    }

    ptr = websGetVar(wp, "HighTempThd", "");
    if (strlen(ptr) > 0)
    {
        pMob->HighTempThd.Set(atoi(ptr));
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

void WebCardAction(Webs *wp)
{
    if (isWebPostMethod(wp))
        WebCardSet(wp);
    else
        WebCardGet(wp);
}

