#include <Windows.h>
#include <iostream>

int main() 
{
    system("chcp 1251");
    
    TCHAR pipeName[] = TEXT("\\\\.\\pipe\\MyPipe");

    
    HANDLE pipe = CreateNamedPipe(
        pipeName,                  // ��� ������
        PIPE_ACCESS_DUPLEX,        // ���������� ������
        PIPE_TYPE_MESSAGE |        // ��� ���������
        PIPE_READMODE_MESSAGE |    // ����� ������ ���������
        PIPE_WAIT,                 // ����� ��������
        1,  // ������������ ���������� �����������, ������� ����� ������������ � ������
        0, // ������� ��������  ������
        0, // ������ ��������� ������
        0, // ������ ���� - ����
        NULL // ������� ������������
    );

    if (pipe == INVALID_HANDLE_VALUE) 
    {
        std::cerr << "������ � �������� Pipe. ��� ������: " << GetLastError() << std::endl;
        return 1;
    }

    std::cout << "������ ���� �����������..." << std::endl;

    if (ConnectNamedPipe(pipe, NULL) != FALSE) 
    {
        std::cout << "������ ���������. ��������� ���������..." << std::endl;
    
           
        while (true)
        {
            char buffer[100];
            DWORD bytesRead;
        
            if (ReadFile(pipe, buffer, sizeof(buffer), &bytesRead, NULL) != FALSE)
            {
                if (bytesRead > 0) 
                {
                    
                    if (WriteFile(pipe, buffer, bytesRead, NULL, NULL) == FALSE) 
                    {
                        std::cerr << "������ ��������� � Pipe. ��� ������: " << GetLastError() << std::endl;
                        break;
                    }
                }
            }
            else 
            {
                std::cerr << "������ ������ Pipe. ��� ������: " << GetLastError() << std::endl;
                break;
            }
        }
    }

    
    DisconnectNamedPipe(pipe);
    CloseHandle(pipe);

    std::cout << "������ ��������." << std::endl;

    return 0;
}
