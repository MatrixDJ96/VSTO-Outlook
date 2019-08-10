using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.IO;

namespace VSTO_Outlook_Installer
{
    class Program
    {
        static string AddinID = "";

        static List<string> List = new List<string>() { "Software", "Microsoft", "Office", "Outlook", "Addins" };

        static RegistryKey LocateRegistryKey(RegistryKey registryKey, int index = 0)
        {
            RegistryKey newRegistryKey = null;

            if (index < List.Count)
            {
                foreach (var key in registryKey.GetSubKeyNames())
                {
                    if (key == List[index])
                    {
                        // Eseguo ricorsivamete la funzione per verificare tutto le chiave di List
                        newRegistryKey = LocateRegistryKey(registryKey.OpenSubKey(List[index], true), ++index);
                        break;
                    }
                }
            }
            else
            {
                // Prendo quella precedentemente impostata dalla funzione ricorsiva
                newRegistryKey = registryKey;
            }

            return newRegistryKey;
        }

        static void Main(string[] args)
        {
            if (args.Length >= 1 && File.Exists(args[0]))
            {
                // Imposto l'ID relativo al VSTO AddIn
                if (args.Length > 1)
                {
                    AddinID = args[1];
                }
                else
                {
                    AddinID = Path.GetFileNameWithoutExtension(args[0]);
                }

                // Controllo se Outlook è installato nel sistema
                if (LocateRegistryKey(Registry.CurrentUser) is RegistryKey registryKey)
                {
                    List<string> subKeys = new List<string>(registryKey.GetSubKeyNames());

                    // Installo il VSTO AddIn
                    using (var addinKey = (string.IsNullOrWhiteSpace(subKeys.Find(x => x.Equals(AddinID))) ? registryKey.CreateSubKey(AddinID) : registryKey.OpenSubKey(AddinID, true)))
                    {
                        addinKey.SetValue("Description", AddinID, RegistryValueKind.String);
                        addinKey.SetValue("FriendlyName", AddinID, RegistryValueKind.String);
                        addinKey.SetValue("LoadBehavior", 3, RegistryValueKind.DWord);
                        addinKey.SetValue("Manifest", new Uri(args[0]), RegistryValueKind.String);
                    }
                }
                else
                {
                    Console.WriteLine("Impossibile trovare Outlook");
                }
            }
            else
            {
                Console.WriteLine("Eseguire il programma con argomento il path relativo al plugin da installare");
            }
        }
    }
}
