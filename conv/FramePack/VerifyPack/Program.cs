using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace VerifyPack
{
    class Program
    {
        static void Main(string[] args)
        {
            string path = @"D:\Security\VMSHARED\Development\BadAppleDOS\conv\badapple.bin";
            var freader = File.Open(path, FileMode.Open, FileAccess.Read, FileShare.None);
            byte[] buf = new byte[4];

            freader.Read(buf, 0, 2);
            ushort count = BitConverter.ToUInt16(buf, 0);

            byte fps = (byte) freader.ReadByte();
            Console.WriteLine($"{count} frame(s) at {fps} fps");

            for (int f = 0; f < count; f++)
            {
                freader.Read(buf, 0, 2);
                ushort chunkCount = BitConverter.ToUInt16(buf, 0);
                Console.WriteLine($"frame {f} with {chunkCount} chunk(s)");

                for (int c = 0; c < chunkCount; c++)
                {
                    byte data =(byte) freader.ReadByte();
                    freader.Read(buf, 0, 2);
                    ushort repeat = BitConverter.ToUInt16(buf, 0);
                    Console.Write($"(D{data}, R{repeat})");
                }
                Console.WriteLine();
                Thread.Sleep(1000);
            }
        }
    }
}
