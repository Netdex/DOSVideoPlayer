using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace AudioPack
{
    class Program
    {
        static void Main(string[] args)
        {
            //string path = @"D:\Security\VMSHARED\Development\code\BadAppleDOS\conv\midi";
            //string filename = "ba_mono";
            //int fps = 96;
            string path = args[0];
            string filename = args[1];
            int fps = int.Parse(args[2]);

            string filepath = Path.Combine(path, filename + ".csv");
            double resolution = 1.0 / fps;
            string[] data = File.ReadAllLines(filepath);
            double tickpsec = -1;

            var fstream = File.Open(Path.Combine(path, $"audio_{filename}.bin"), FileMode.Create, FileAccess.Write, FileShare.None);
            var eventBuffer = new MemoryStream();
            ushort eventCount = 0;
            ushort lastFrame = 0;
            byte lastAction = 0;
            ushort lastFreq = 0;

            foreach (string line in data)
            {
                string[] linedata = line.Split(',');
                for (int i = 0; i < linedata.Length; i++)
                    linedata[i] = linedata[i].Trim();

                if (linedata[2] == "Note_on_c" || linedata[2] == "Note_off_c")
                {
                    double time = double.Parse(linedata[1]) / tickpsec;
                    ushort frame = (ushort)(time / resolution);
                    int note = int.Parse(linedata[4]);
                    ushort freq = (ushort)GetFrequency(note);

                    if (frame != lastFrame && lastFrame != 0)
                    {
                        eventBuffer.Write(BitConverter.GetBytes(lastFrame), 0, 2);

                        eventBuffer.WriteByte(lastAction);
                        if (lastAction == 1)
                            eventBuffer.Write(BitConverter.GetBytes(lastFreq), 0, 2);

                        eventCount++;
                    }
                    if (linedata[2] == "Note_off_c")
                        lastAction = 0;
                    else
                        lastAction = 1;
                    lastFrame = frame;
                    lastFreq = freq;
                }
                else if (linedata[2] == "Tempo")
                {
                    tickpsec = 1000000 / double.Parse(linedata[3]) * 24 * 4;
                }
            }
            eventBuffer.Write(BitConverter.GetBytes(lastFrame), 0, 2);
            eventBuffer.WriteByte(lastAction);
            if (lastAction == 1)
                eventBuffer.Write(BitConverter.GetBytes(lastFreq), 0, 2);
            eventCount++;

            fstream.Write(BitConverter.GetBytes(eventCount), 0, 2);
            fstream.WriteByte((byte)fps);
            eventBuffer.WriteTo(fstream);
        }

        public static int GetFrequency(int key)
        {
            return (int)(Math.Pow(2, (key - 69) / 12.0) * 440);
        }
    }
}
