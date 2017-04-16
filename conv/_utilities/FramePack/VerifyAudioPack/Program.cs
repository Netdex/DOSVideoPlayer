using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace VerifyAudioPack
{
    class Program
    {
        static void Main(string[] args)
        {
            string file = @"D:\Security\VMSHARED\Development\BadAppleDOS\conv\midi\audio.bin";
            int fps = 15;

            int delay = (int) ((1.0 / fps) * 1000);
            var fstream = File.Open(file, FileMode.Open, FileAccess.Read, FileShare.None);
            byte[] buf = new byte[4];
            fstream.Read(buf, 0, 2);
            ushort events = BitConverter.ToUInt16(buf, 0);
            ushort lastFrame = 0;
            for (int e = 0; e < events; e++)
            {
                fstream.Read(buf, 0, 2);
                ushort frame = BitConverter.ToUInt16(buf, 0);

                int inst = fstream.ReadByte();
                if (inst == 1)
                {
                    fstream.Read(buf, 0, 2);
                    ushort freq = BitConverter.ToUInt16(buf, 0);
                    Console.Write(frame + " ");
                }
                Thread.Sleep((frame - lastFrame) * delay);
                lastFrame = frame;
                
            }
        }
    }
}
