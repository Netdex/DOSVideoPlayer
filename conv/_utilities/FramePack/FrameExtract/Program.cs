using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using SimplePaletteQuantizer.Helpers;
using SimplePaletteQuantizer.Quantizers;
using SimplePaletteQuantizer.Quantizers.Octree;
using SimplePaletteQuantizer.Quantizers.Uniform;

namespace FrameExtract
{
    class Program
    {
        public static unsafe void Main(string[] args)
        {
            //string path = @"D:\Security\VMSHARED\Development\code\BadAppleDOS\conv\frames\badapple w lyric";
            //string prefix = "bal";
            //string extension = "png";
            //ushort count = 5259;
            //byte frameRate = 24;
            const byte RESERVED_BYTES = 16;
            string path = args[0];
            string prefix = args[1];
            string extension = args[2];
            ushort count = ushort.Parse(args[3]);
            byte frameRate = byte.Parse(args[4]);
            byte colors = byte.Parse(args[5]);

            Console.WriteLine("FRAMEEXTRACT - it's FramePack without the packing part");

            var binPath = Path.Combine(Environment.CurrentDirectory, $"{prefix}.bin");
            var ostream = File.Open(binPath, FileMode.Create, FileAccess.Write, FileShare.None);

            Console.WriteLine($"reading {count} frame(s) from {path} of {prefix} with type {extension}\n" +
                              $"writing output to {binPath}");

            ostream.Write(BitConverter.GetBytes(count), 0, 2);
            ostream.WriteByte(frameRate);

            byte[] rawpalette = new byte[256 * 3];
            byte[] framebytes = new byte[320 * 200];
            
            var oq = new OctreeQuantizer();

            for (int f = 0; f < count; f++)
            {
                Console.Write($"frame {f + 1} / {count}");
                string fileName = $"{prefix}_{f:D6}.{extension}";
                string filePath = Path.Combine(path, fileName);
                using (Bitmap bitmap = (Bitmap)Image.FromFile(filePath))
                {
                    oq.Prepare(null);
                    var data = bitmap.LockBits(new Rectangle(Point.Empty, bitmap.Size), ImageLockMode.ReadOnly,
                        PixelFormat.Format32bppPArgb);
                    
                    for (int offset = 0; offset < data.Width * data.Height; offset++)
                    {
                        byte* ptr = (byte*)data.Scan0 + offset * 4;
                        byte b = ptr[0];
                        byte g = ptr[1];
                        byte r = ptr[2];

                        oq.AddColor(Color.FromArgb(r, g, b), offset % data.Width, offset / data.Width);
                    }

                    Color[] palette = oq.GetPalette(colors - RESERVED_BYTES).ToArray();

                    for (int i = 0; i < palette.Length; i++)
                    {
                        rawpalette[i * 3] = palette[i].R;
                        rawpalette[i * 3 + 1] = palette[i].G;
                        rawpalette[i * 3 + 2] = palette[i].B;
                    }

                    ostream.WriteByte((byte) (palette.Length - 1));
                    ostream.Write(rawpalette, 0, palette.Length * 3);

                    for (int offset = 0; offset < data.Width * data.Height; offset++)
                    {
                        byte* ptr = (byte*)data.Scan0 + offset * 4;
                        byte b = ptr[0];
                        byte g = ptr[1];
                        byte r = ptr[2];

                        byte d = (byte) (GetPaletteIndex(oq, offset % data.Width, offset / data.Width, r, g, b) + RESERVED_BYTES);
                        framebytes[offset] = d;
                    }
                    ostream.Write(framebytes, 0, framebytes.Length);
                    oq.Finish();
                }
                Console.Write('\r');
            }

            Console.WriteLine("\nall operations completed");
        }

        public static byte GetPaletteIndex(IColorQuantizer q, int x, int y, int r, int g, int b)
        {
            return (byte) q.GetPaletteIndex(Color.FromArgb(r, g, b), x, y);
        }
        public static byte GetBinaryData(byte r, byte g, byte b)
        {
            return (byte)(16 + (r + g + b) / 3 / 16);
        }
    }
}