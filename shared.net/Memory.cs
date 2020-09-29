using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace AntiLeague
{
    internal class Memory
    {
        internal static T Read<T>(int Address)
        {
            int size = Marshal.SizeOf<T>();
            byte[] buf = new byte[size];
            bool res = Native.Memory.ReadProcessMemory(Process.GetProcessesByName("League of Legends").FirstOrDefault().Handle, (IntPtr)Address, buf, size, out var lpRead);
#if DEBUG
            if (!res)
            {
                var color = Console.ForegroundColor;
                Console.ForegroundColor = ConsoleColor.Red;
                Console.WriteLine($"Error : read process memory failed - 0x{Address:X}");
                Console.ForegroundColor = color;
            }
#endif
            var ptr = Marshal.AllocHGlobal(size);
            Marshal.Copy(buf, 0, ptr, size);
            var _struct = Marshal.PtrToStructure<T>(ptr);
            Marshal.FreeHGlobal(ptr);
            return _struct;
        }

        public static string ReadString(int address, Encoding Encoding)
        {
            byte[] dataBuffer = new byte[512];
            IntPtr bytesRead = IntPtr.Zero;

            Native.Memory.ReadProcessMemory(System.Diagnostics.Process.GetProcessesByName("League of Legends").FirstOrDefault().Handle, (IntPtr)address, dataBuffer, dataBuffer.Length, out bytesRead);

            if (bytesRead == IntPtr.Zero)
            {
                return string.Empty;
            }

            return Encoding.GetString(dataBuffer).Split('\0')[0];
        }

        public static Matrix ReadMatrix(int address)
        {
            Matrix tmp = Matrix.Zero;

            byte[] buf = new byte[64];
            IntPtr byteRead;

            Native.Memory.ReadProcessMemory(Process.GetProcessesByName("League of Legends").FirstOrDefault().Handle, (IntPtr)address, buf, 64, out byteRead);

            if (byteRead == IntPtr.Zero)
            {
                return new Matrix();
            }

            tmp.M11 = BitConverter.ToSingle(buf, (0 * 4));
            tmp.M12 = BitConverter.ToSingle(buf, (1 * 4));
            tmp.M13 = BitConverter.ToSingle(buf, (2 * 4));
            tmp.M14 = BitConverter.ToSingle(buf, (3 * 4));

            tmp.M21 = BitConverter.ToSingle(buf, (4 * 4));
            tmp.M22 = BitConverter.ToSingle(buf, (5 * 4));
            tmp.M23 = BitConverter.ToSingle(buf, (6 * 4));
            tmp.M24 = BitConverter.ToSingle(buf, (7 * 4));

            tmp.M31 = BitConverter.ToSingle(buf, (8 * 4));
            tmp.M32 = BitConverter.ToSingle(buf, (9 * 4));
            tmp.M33 = BitConverter.ToSingle(buf, (10 * 4));
            tmp.M34 = BitConverter.ToSingle(buf, (11 * 4));

            tmp.M41 = BitConverter.ToSingle(buf, (12 * 4));
            tmp.M42 = BitConverter.ToSingle(buf, (13 * 4));
            tmp.M43 = BitConverter.ToSingle(buf, (14 * 4));
            tmp.M44 = BitConverter.ToSingle(buf, (15 * 4));

            return tmp;
        }

        public static Vector3 Read3DVector(int address)
        {
            Vector3 tmp = new Vector3();

            byte[] Buffer = new byte[12];
            IntPtr ByteRead;

            Native.Memory.ReadProcessMemory(Process.GetProcessesByName("League of Legends").FirstOrDefault().Handle, (IntPtr)(address + Offset.Object.Pos), Buffer, 12, out ByteRead);

            tmp.X = BitConverter.ToSingle(Buffer, (0 * 4));
            tmp.Y = BitConverter.ToSingle(Buffer, (1 * 4));
            tmp.Z = BitConverter.ToSingle(Buffer, (2 * 4));

            return tmp;
        }
    }
}
