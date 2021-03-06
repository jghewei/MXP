#include "DanOS.h"
#include "FileMgmtMob.h"
#include "Web.h"

void WebFileMgmtGet(Webs *wp)
{
    int inst = 0, num = FileMgmtMob::GetInstNum();
    char *pid = websGetVar(wp, "id", "");
    if (strlen(pid) > 0)
    {
        inst = atoi(pid);
        num = inst + 1;
    }

    CJson webs;
    for (; inst < num; inst++)
    {
        FileMgmtMob *pMob = FileMgmtMob::GetInst(inst);
        if (pMob == NULL) continue;

        webs.AddEntry();
        webs.AddValue("Key", pMob->key.name());
        webs.AddValue("ServerIpAddress", pMob->ServerIpAddress.Get());
        webs.AddValue("ServerProtocol", pMob->ServerProtocol.Get());
        webs.AddValue("FilePathName", pMob->FilePathName.Get());
        webs.AddValue("Action", pMob->Action.Get());
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

void WebFileMgmtSet(Webs *wp)
{
    int inst = 0;
    char *ptr = NULL;

    ptr = websGetVar(wp, "id", "");
    if (strlen(ptr) > 0)
    {
        inst = atoi(ptr);
    }

    FileMgmtMob *pMob = FileMgmtMob::GetInst(inst);
    if (pMob == NULL) goto error_exit;

    ptr = websGetVar(wp, "ServerIpAddress", "");
    if (strlen(ptr) > 0)
    {
        pMob->ServerIpAddress.Set(ptr);
    }

    ptr = websGetVar(wp, "ServerProtocol", "");
    if (strlen(ptr) > 0)
    {
        pMob->ServerProtocol.Set(atoi(ptr));
    }

    ptr = websGetVar(wp, "FilePathName", "");
    if (strlen(ptr) > 0)
    {
        pMob->FilePathName.Set(ptr);
    }

    ptr = websGetVar(wp, "Action", "");
    if (strlen(ptr) > 0)
    {
        pMob->Action.Set(atoi(ptr));
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

void WebFileMgmtAction(Webs *wp)
{
    if (isWebPostMethod(wp))
        WebFileMgmtSet(wp);
    else
        WebFileMgmtGet(wp);
}

