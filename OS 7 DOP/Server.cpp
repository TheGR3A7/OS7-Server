#include <Windows.h>
#include <iostream>

int main() 
{
    system("chcp 1251");
    
    TCHAR pipeName[] = TEXT("\\\\.\\pipe\\MyPipe");

    
    HANDLE pipe = CreateNamedPipe(
        pipeName,                  // Имя канала
        PIPE_ACCESS_DUPLEX,        // Дуплексный доступ
        PIPE_TYPE_MESSAGE |        // Тип сообщения
        PIPE_READMODE_MESSAGE |    // Режим чтения сообщения
        PIPE_WAIT,                 // Режим ожидания
        1,  // Максимальное количество экземпляров, которые могут подключаться к каналу
        0, // Размеру входного  буфера
        0, // Размер выходного буфера
        0, // режиму тайм - аута
        NULL // Атрибут безопасности
    );

    if (pipe == INVALID_HANDLE_VALUE) 
    {
        std::cerr << "Ошибка в создании Pipe. Код ошибки: " << GetLastError() << std::endl;
        return 1;
    }

    std::cout << "Сервер ждет подключения..." << std::endl;

    if (ConnectNamedPipe(pipe, NULL) != FALSE) 
    {
        std::cout << "Клиент подключен. Ожидается сообщение..." << std::endl;
    
           
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
                        std::cerr << "Ошибка написания в Pipe. Код ошибки: " << GetLastError() << std::endl;
                        break;
                    }
                }
            }
            else 
            {
                std::cerr << "Ошибка чтения Pipe. Код ошибки: " << GetLastError() << std::endl;
                break;
            }
        }
    }

    
    DisconnectNamedPipe(pipe);
    CloseHandle(pipe);

    std::cout << "Сервер отключен." << std::endl;

    return 0;
}
