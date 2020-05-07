#include "DanOS.h"
#include "PsuMob.h"
#include "Web.h"

void WebPsuGet(Webs *wp)
{
    int inst = 0, num = PsuMob::GetInstNum();
    char *pid = websGetVar(wp, "id", "");
    if (strlen(pid) > 0)
    {
        inst = atoi(pid);
        num = inst + 1;
    }

    CJson webs;
    for (; inst < num; inst++)
    {
        PsuMob *pMob = PsuMob::GetInst(inst);
        if (pMob == NULL) continue;

        webs.AddEntry();
        webs.AddValue("Key", pMob->key.name());
        webs.AddValue("State", pMob->State.Get());

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

void WebPsuSet(Webs *wp)
{
    int inst = 0;
    char *ptr = NULL;

    ptr = websGetVar(wp, "id", "");
    if (strlen(ptr) > 0)
    {
        inst = atoi(ptr);
    }

    PsuMob *pMob = PsuMob::GetInst(inst);
    if (pMob == NULL) goto error_exit;

    pMob->Save();

error_exit:
    std::string resp = WebFormResponse(true, "command complete");

    websSetStatus(wp, 200);
    websWriteHeaders(wp, resp.size(), 0);
    websWriteEndHeaders(wp);
    websWrite(wp, (char*)resp.c_str());
    websDone(wp);
}

void WebPsuAction(Webs *wp)
{
    if (isWebPostMethod(wp))
        WebPsuSet(wp);
    else
        WebPsuGet(wp);
}

