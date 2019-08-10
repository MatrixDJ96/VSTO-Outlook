#include "pch.h"
#include <iostream>
#include <Windows.h>

using namespace std;

/* .NET Framework https://docs.microsoft.com/it-it/dotnet/framework/deployment/deployment-guide-for-developers */
/* RegOpenKeyEx: https://docs.microsoft.com/it-it/windows/win32/api/winreg/nf-winreg-regopenkeyexw */
/* RegGetValue: https://docs.microsoft.com/it-it/windows/win32/api/winreg/nf-winreg-reggetvaluew */
/* LSTATUS: https://docs.microsoft.com/en-us/windows/win32/debug/system-error-codes */

LSTATUS GetValueFromRegistry(HKEY hKey, LPCWSTR lpSubKey, LPCWSTR lpValue, DWORD& pdwType, DWORD& pcbData, PVOID& pvData)
{
	// hKey: chiave principale
	// lpSubKey: chiave secondaria
	// lpValue: chiave da leggere

	// Chiave che verrà aperta da RegOpenKeyEx
	HKEY phkResult;

	// Effettuo l'accesso alla chiave di registro
	auto status = RegOpenKeyEx(hKey, lpSubKey, (DWORD)0, KEY_READ, &phkResult);

	if (status == ERROR_SUCCESS) {
		// Variabile che indicherà il tipo relativa alla chiave lpValue
		pdwType = 0;
		// Variabile che indicherà la dimensione relativa della chiave lpValue
		pcbData = 0;

		// Leggo le informazione relative alla chiave lpValue
		status = RegGetValue(phkResult, nullptr, lpValue, RRF_RT_ANY, &pdwType, nullptr, &pcbData);

		if (status == ERROR_SUCCESS) {
			// Preparo il buffer per l'estrazione del valore della chiave lpValue
			pvData = new BYTE[pcbData];

			// Estraggo il valore della chiave lpValue
			status = RegGetValue(phkResult, nullptr, lpValue, RRF_RT_ANY, &pdwType, pvData, &pcbData);
		}

		// Chiudo la chiave del registro
		RegCloseKey(phkResult);
	}

	// Ritorno il risultato dell'ultima operazione eseguita
	return status;
}

int main()
{
	DWORD pdwType; // key type
	DWORD pcbData; // key size
	PVOID pvData;  // key value

	auto status = GetValueFromRegistry(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\NET Framework Setup\\NDP\\v4\\Full", L"Release", pdwType, pcbData, pvData);

	if (status == ERROR_SUCCESS) {
		if (pdwType == REG_DWORD && *(DWORD*)pvData >= 379893) {
			wcout << L".NET Framework 4.5.2 è stato correttamente installato" << endl;
		}
		else {
			wcout << L".NET Framework 4.5.2 non è installato nel tuo sistema!" << endl;
			wcout << L"Installazione in corso...";

			// TODO: Installazione di .NET Framework
		}
	}
	else {
		wcerr << L"Impossibile leggere/aprire il registro di sistema (LSTATUS: " << status << ")" << endl;
	}
}
