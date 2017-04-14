using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FramePack
{
    class Program
    {
        public static unsafe void Main(string[] args)
        {
            string path = @"D:\Security\VMSHARED\Development\BadAppleDOS\conv\frames";
            string prefix = "badapple";
            string extension = "png";
            ushort count = 3288;
            byte frameRate = 15;

            Console.WriteLine("FRAMEPACK");
            
            var binPath = Path.Combine(path, "..", $"{prefix}.bin");
            var ostream = File.Open(binPath, FileMode.Create, FileAccess.Write, FileShare.None);

            Console.WriteLine($"reading {count} frame(s) from {path} of {prefix} with type {extension}\n" +
                              $"writing output to {binPath}");

            ostream.Write(BitConverter.GetBytes(count), 0, 2);
            ostream.WriteByte(frameRate);

            for (int f = 0; f < count; f++)
            {
                Console.Write($"frame {f} / {count}");
                string fileName = $"{prefix}_{f:D6}.{extension}";
                string filePath = Path.Combine(path, fileName);
                using (Bitmap bitmap = (Bitmap)Image.FromFile(filePath))
                {
                    
                    var data = bitmap.LockBits(new Rectangle(Point.Empty, bitmap.Size), ImageLockMode.ReadOnly,
                        PixelFormat.Format32bppPArgb);

                    using (MemoryStream tempBuffer = new MemoryStream())
                    {
                        ushort chunkCount = 0;
                        byte lastData = 0;
                        ushort repeatCount = 1;
                        for (int offset = 0; offset < data.Width * data.Height; offset++)
                        {
                            byte* ptr = (byte*)data.Scan0 + offset * 4;
                            byte b = ptr[0];
                            byte g = ptr[1];
                            byte r = ptr[2];

                            var d = GetBinaryData(r, g, b);
                            if (offset != 0)
                            {
                                if (lastData == d)
                                {
                                    repeatCount++;
                                }
                                else
                                {
                                    tempBuffer.WriteByte(lastData);
                                    tempBuffer.Write(BitConverter.GetBytes(repeatCount), 0, 2);
                                    repeatCount = 1;
                                    chunkCount++;
                                }
                            }
                            lastData = d;
                        }
                        tempBuffer.WriteByte(lastData);
                        tempBuffer.Write(BitConverter.GetBytes(repeatCount), 0, 2);
                        chunkCount++;

                        ostream.Write(BitConverter.GetBytes(chunkCount), 0, 2);
                        tempBuffer.WriteTo(ostream);
                    }
                }
                Console.Write('\r');
            }
            Console.WriteLine("\nall operations completed");
        }

        public static byte GetBinaryData(byte r, byte g, byte b)
        {
            if ((r + g + b) / 3 > 128)
                return 15;

            return 0;
        }
    }
}
