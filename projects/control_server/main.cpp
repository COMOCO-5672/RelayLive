// sever.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include "util.h"
#include "utilc.h"
#include "server.h"
#include "ipc.h"
#include "uv.h"
#include "easylog.h"

using namespace util;

int main(int argc, char* argv[])
{
    if(argc != 2)
        return -1;
    int port = atoi(argv[1]);

    /** Dump���� */
    //CMiniDump dump("control_server.dmp");

    /** ������־�ļ� */
    char path[MAX_PATH];
    sprintf(path, "./log/control_server.txt");
    Log::open(Log::Print::both, uvLogPlus::Level::Debug, path);
    Log::debug("version: %s %s", __DATE__, __TIME__);

    /** ���������ļ� */
    if (!Settings::loadFromProfile("./config.txt"))
        Log::error("Settings::loadFromProfile failed");
    else
        Log::debug("Settings::loadFromProfile ok");

    //����cpu��������libuv�̳߳ص��߳�����
    uv_cpu_info_t* cpu_infos;
    int count;
    int err = uv_cpu_info(&cpu_infos, &count);
    if (err) {
        Log::warning("fail get cpu info: %s",uv_strerror(err));
    } else {
        char szThreadNum[10] = {0};
        sprintf(szThreadNum, "%d", count*2+1);
        Log::debug("thread pool size is %s", szThreadNum);
        //���û���������ֵ
        uv_os_setenv("UV_THREADPOOL_SIZE", szThreadNum);
    }
    uv_free_cpu_info(cpu_infos, count);

    IPC::Init();

    /** ����һ��http������ */
    Server::Init(port);

    Log::debug("control sever @%d start success\r\n", port);

    Sleep(INFINITE);
    return 0;
}