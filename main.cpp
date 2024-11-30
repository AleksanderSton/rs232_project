#include <iostream>
#include <windows.h>
HANDLE commDev;
int buffSize = 1024;
bool writeSerialPort(void* buffer, unsigned long numberOfBytesToWrite){
    DWORD numberOfBytesWritten = 0;
    if(WriteFile(commDev, buffer, numberOfBytesToWrite, &numberOfBytesWritten, nullptr) == 0){
        std::cout << "Błąd wysyłania danych!" << std::endl;
        return false;
    }
        return true;
}

bool readSerialPort(void* buffer, unsigned long numberOfBytesToRead){
    DWORD numberOfBytesRead=0;
    if (ReadFile (commDev, buffer, numberOfBytesToRead, &numberOfBytesRead, 0)){
        return true;
    }
    return false;
}

int main() {
    std::string portName ;
    std::cout<<"Podaj nazwe portu:"<<std::endl;
    std::cin>>portName;
    commDev =  CreateFile(portName.c_str(),
                                 GENERIC_READ | GENERIC_WRITE,
                                 0,
                                 NULL,
                                 OPEN_EXISTING,
                                 0,
                                 NULL);
    if(commDev==INVALID_HANDLE_VALUE){
        std::cout<<"PORT NIE ZOSTAL OTWARTY DO TRANSMISJI!"<<std::endl<<std::endl;
    }
    else{
        std::cout<<"PORT ZOSTAL OTWARTY DO TRANSMISJI!"<<std::endl<<std::endl;
        DCB dcb;
        dcb.DCBlength = sizeof(dcb); // aktualny rozmiar struktury DCB
        GetCommState(commDev, &dcb); // udostępnienie aktualnych parametrów DCB
        std::cout<<"Poprzednie ustawienia:"<<std::endl;
        std::cout<<"\t- Predkosc transmisji: "<< dcb.BaudRate<<std::endl;
        std::cout<<"\t- Parzystosc: "<<dcb.fParity<<std::endl;
        std::cout<<"\t- Bity danych: "<<int(dcb.ByteSize)<<std::endl;
        std::cout<<"\t- Bity stopu: "<<dcb.StopBits<<std::endl;
        ///timeout-y
        COMMTIMEOUTS timeouts = {};
        timeouts.ReadIntervalTimeout = 0;
        timeouts.ReadTotalTimeoutConstant = 3000;
        timeouts.ReadTotalTimeoutMultiplier = 0;
        timeouts.WriteTotalTimeoutConstant = 0;
        timeouts.WriteTotalTimeoutMultiplier = 0;
        if (!SetCommTimeouts(commDev, &timeouts)) {
            std::cout << "Błąd ustawiania timeout'ów!" << std::endl;
            CloseHandle(commDev);
            return 1;
        }
        char changeSettings;
        std::cout<<"Czy chcesz zmienic ustawienia portu? \n y/n?"<<std::endl;
        std::cin>>changeSettings;
        if(changeSettings == 'y'){
            //
            // dcb.BaudRate = CBR_256000; // prędkość transmisji
            // dcb.fParity = TRUE; // sprawdzanie parzystości
            // dcb.Parity = NOPARITY; // ustawienie parzystości
            // dcb.StopBits = TWOSTOPBITS; // bity stopu
            // dcb.ByteSize = 8; // bity danych
            // dcb.fDtrControl = 1; // np. kontrola linii DTR

            std::cout<< "Wybierz wartsc parametru BaudRate:\n"
            "[1]-CBR_110 | [2]-CBR_300 | [3]-CBR_600 | [4]-CBR_1200 | [5]-CBR_2400 | [6]-CBR_4800 | [7]-CBR_9600 | [8]-CBR_14400 \n "
            "[9]-CBR_19200 | [10]-CBR_38400 | [11]-CBR_56000 | [12]-CBR_57600 | [13]-CBR_115200 | [14]CBR_128000 | [15]-CBR_256000  "<<std::endl;
            int baudRate = 0;
            std::cin>>baudRate;
            switch (baudRate) {
                case 1: dcb.BaudRate = CBR_110; break;
                case 2: dcb.BaudRate = CBR_300; break;
                case 3: dcb.BaudRate = CBR_600; break;
                case 4: dcb.BaudRate = CBR_1200; break;
                case 5: dcb.BaudRate = CBR_2400; break;
                case 6: dcb.BaudRate = CBR_4800; break;
                case 7: dcb.BaudRate = CBR_9600; break;
                case 8: dcb.BaudRate = CBR_14400; break;
                case 9: dcb.BaudRate = CBR_19200; break;
                case 10: dcb.BaudRate = CBR_38400; break;
                case 11: dcb.BaudRate = CBR_56000; break;
                case 12: dcb.BaudRate = CBR_57600; break;
                case 13: dcb.BaudRate = CBR_115200; break;
                case 14: dcb.BaudRate = CBR_128000; break;
                case 15: dcb.BaudRate = CBR_256000; break;
                default: std::cout<<"Niepoprawna wartosc!"<<std::endl;break;
            }

            std::cout<<"Wybierz wartosc parametru fParity:\n"
            "[1]-EVENPARITY | [2]-MARKPARITY | [3]-NOPARITY | [4]-ODDPARITY"<<std::endl;
            int parity=0;
            std::cin>>parity;
            switch (parity) {
                case 1: dcb.fParity = EVENPARITY; break;
                case 2: dcb.fParity = MARKPARITY; break;
                case 3: dcb.fParity = NOPARITY; break;
                case 4: dcb.fParity = ODDPARITY; break;
                default:std::cout<<"Niepoprawna wartosc!"<<std::endl;break;
            }
            std::cout<<"Wybierz wartosc parametru ByteSize:\n"
                       "5 | 6 | 7 | 8"<<std::endl;
            int byteSize = 0;
            std::cin>>byteSize;
            switch (byteSize) {
                case 5: dcb.ByteSize = 5; break;
                case 6: dcb.ByteSize = 6; break;
                case 7: dcb.ByteSize = 7; break;
                case 8: dcb.ByteSize = 8; break;
                default: std::cout<<"Niepoprawna wartosc!"<<std::endl;break;
            }
            std::cout<<"Wybierz wartosc parametru StopBits:\n"
                       "[1]-ONESTOPBIT | [2]-ONE5STOPBITS | [3]-TWOSTOPBITS"<<std::endl;
            int stopBits = 0;
            std::cin>>stopBits;
            switch (stopBits) {
                case 1: dcb.StopBits = ONESTOPBIT; break;
                case 2: dcb.StopBits = ONE5STOPBITS; break;
                case 3: dcb.StopBits = TWOSTOPBITS; break;
            }
            SetCommState(commDev,&dcb);
            GetCommState(commDev,  &dcb);
            std::cout<<"Ustawienia po zmianie:"<<std::endl;
            std::cout<<"\t- Predkosc transmisji: "<< dcb.BaudRate<<std::endl;
            std::cout<<"\t- Parzystosc: "<<dcb.fParity<<std::endl;
            std::cout<<"\t- Bity danych: "<<int(dcb.ByteSize)<<std::endl;
            std::cout<<"\t- Bity stopu: "<<dcb.StopBits<<std::endl;
        }
        int choice;
        std::cout<<"Wybierz tryb wyslania"<<std::endl;
        //std::cout<<"[1] Wyslanie litery"<<std::endl;
        std::cout<<"[1] Wyslanie lancucha znakow"<<std::endl;
        std::cout<<"[2] Wyslanie tresci pliku"<<std::endl;
        std::cin>>choice;
        // if(choice == 1){
        //     ///Wysłanie liter
        //     char letter[2];
        //     std::cout<<"Podaj znak do wyslania: "<<std::endl;
        //     std::cin>>letter;
        //     letter[1] ='\0';
        //     char* buffIn = new char;
        //     strcpy(buffIn,letter);
        //     if(writeSerialPort(buffIn,2))
        //         std::cout<<"Pomyslnie wyslano litere!"<<std::endl;
        //     else
        //         std::cout<<"Blad w wyslaniu litery!"<<std::endl;
        //     ///Odczyt liter
        //     char *buffOut = new char;
        //     if(readSerialPort(buffOut,2))
        //         std::cout<<"Pomyslnie odebrano litere!"<<std::endl;
        //     else
        //         std::cout<<"Blad w odbiorze litery!"<<std::endl;
        //     delete buffIn;
        //     delete buffOut;
        // }
        if(choice == 1){
            ///Wysłanie tekstu
            std::string text;
            std::cout<<"Podaj teskt do wyslania!"<<std::endl;
            std::cin.ignore();
            getline(std::cin,text);
            int buffSizeText = text.size()+1;
            char* buffIn = new char[buffSizeText];
            strcpy(buffIn,text.c_str());
            if(writeSerialPort(buffIn,buffSizeText))
                std::cout<<"Pomyslnie wyslano tekst!"<<std::endl;
            else
                std::cout<<"Blad w wyslaniu tekstu!"<<std::endl;
            ///Odbieranie tekstu
            char* buffOut = new char[buffSizeText];
            if(readSerialPort(buffOut,buffSizeText) == 0)
                std::cout<<"Blad w odbieraniu tekstu!"<<std::endl;
            std::string result(buffOut);
            std::cout<<"Odebrano tekst o tresci: "<<result<<std::endl;
            delete buffIn;
            delete buffOut;
        }
        else if(choice == 2) {
            //przesyłanie pliku
            std::string pathOut;
            std::string pathIn;
            std::cout<<"Podaj lokalizacje pliku, ktory chcesz skopiowac"<<std::endl;
            std::cin.ignore();
            getline(std::cin,pathOut);
            std::cout<<"Podaj lokalizacje docelowa pliku"<<std::endl;
            getline(std::cin,pathIn);
            FILE *src = fopen(pathOut.c_str(),"rb");
            FILE *dst = fopen(pathIn.c_str(),"wb");
            fseek(src,0,SEEK_END);
            const long size = ftell(src);
            int frame = size/buffSize;
            fseek(src,0,SEEK_SET);
            char* buffIn = new char[buffSize];
            char* buffOut = new char[buffSize];
            for(int i = 0; i < frame; i++) {
                fread(buffOut,buffSize,1,src);
                writeSerialPort(buffOut,buffSize);
                if(readSerialPort(buffIn,buffSize) == 0) {
                    std::cout << "Błąd w odbiorze danych!" << std::endl;
                    break;
                }
                fwrite(buffIn,buffSize,1,dst);
                std::cout<<i<<"/"<<frame<<std::endl;
            }
            std::cout<<frame<<"/"<<frame<<std::endl;
            int remainingBytes = size % buffSize;
            if (remainingBytes > 0) {
                fread(buffOut, remainingBytes, 1, src);
                writeSerialPort(buffOut, remainingBytes);
                readSerialPort(buffIn, remainingBytes);
                fwrite(buffIn, remainingBytes, 1, dst);
            }
            // if(writeSerialPort(buffIn,buffSize))
            //     std::cout<<"Pomyslnie wyslano tekst z pliku o sciezce: "<<pathOut<<"!"<<std::endl;
            // else
            //     std::cout<<"Blad w wyslaniu tekstu z pliku!"<<std::endl;
            // if(readSerialPort(buffOut,buffSize))
            //     std::cout<<"Pomyslnie odebrano tekst i wpisano do pliku o sciezce: "<<pathIn<<"!"<<std::endl;
            // else
            //     std::cout<<"Blad w odbieraniu tekstu!"<<std::endl;
            fclose(src);
            fclose(dst);
            delete buffIn;
            delete buffOut;
        }
    }
    CloseHandle(commDev);
    return 0;
}