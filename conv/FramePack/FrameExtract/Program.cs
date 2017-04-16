using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FrameExtract
{
    class Program
    {
        public static unsafe void Main(string[] args)
        {
            string path = @"D:\Security\VMSHARED\Development\code\BadAppleDOS\conv\frames\badapple";
            string prefix = "badapple";
            string extension = "png";
            ushort count = 3288;
            byte frameRate = 15;

            Console.WriteLine("FRAMEEXTRACT - it's FramePack without the packing part");

            var binPath = Path.Combine(path, "..", $"{prefix}.bin");
            var ostream = File.Open(binPath, FileMode.Create, FileAccess.Write, FileShare.None);

            Console.WriteLine($"reading {count} frame(s) from {path} of {prefix} with type {extension}\n" +
                              $"writing output to {binPath}");

            ostream.Write(BitConverter.GetBytes(count), 0, 2);
            ostream.WriteByte(frameRate);

            byte[] framebytes = new byte[320 * 200];
            for (int f = 0; f < count; f++)
            {
                Console.Write($"frame {f + 1} / {count}");
                string fileName = $"{prefix}_{f:D6}.{extension}";
                string filePath = Path.Combine(path, fileName);
                using (Bitmap bitmap = (Bitmap)Image.FromFile(filePath))
                {
                    var data = bitmap.LockBits(new Rectangle(Point.Empty, bitmap.Size), ImageLockMode.ReadOnly,
                        PixelFormat.Format32bppPArgb);
                    
                    for (int offset = 0; offset < data.Width * data.Height; offset++)
                    {
                        byte* ptr = (byte*)data.Scan0 + offset * 4;
                        byte b = ptr[0];
                        byte g = ptr[1];
                        byte r = ptr[2];

                        byte d = GetBinaryData(r, g, b);
                        framebytes[offset] = d;
                    }
                    ostream.Write(framebytes, 0, framebytes.Length);
                }
                Console.Write('\r');
            }
            


            Console.WriteLine("\nall operations completed");
        }

        public static byte GetBinaryData(byte r, byte g, byte b)
        {
            return (byte)(16 + (r + g + b) /3 /16);
        }
    }
}