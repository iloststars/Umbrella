package com.agoda.umbrella.utils;

import com.agoda.umbrella.StubApp;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.util.zip.CRC32;

public class Crc32Editor
{
	private static long[] crc32Table = new long[256];

	static {
		long crcValue;
		for (int i = 0; i < 256; i++)
		{
			crcValue = i;
			for (int j = 0; j < 8; j++)
			{
				if ((crcValue & 1) == 1)
				{
					crcValue = crcValue >> 1;
					crcValue = 0x00000000edb88320L ^ crcValue;
				}
				else
				{
					crcValue = crcValue >> 1;
				}
			}
			crc32Table[i] = crcValue;
		}
	}

	public static boolean editFile(String path, String newCrc32Str)
	{
		path = StubApp.getContext().getFilesDir().getAbsolutePath()+"/"+path;
		try
		{
			long newCrc = Long.valueOf(newCrc32Str, 16);
			while (getFileCrc32(path) != newCrc)
			{
				FileOutputStream fos = new FileOutputStream(path, true);
				fos.write(getAddBytes(getFileCrc32(path), newCrc));
				fos.close();
			}
			return true;
		}
		catch (Exception e)
		{
			Tools.showException(e);
			return false;
		}
	}

	public static long getFileCrc32(String path)
	{
		long crcValue = -1;
		try
		{
			CRC32 crc = new CRC32();
			FileInputStream fis = new FileInputStream(path);
			int i;
			byte[] buf = new byte[1024];
			while ((i = fis.read(buf)) != -1)
			{
				crc.update(buf, 0, i);
			}
			fis.close();
			crcValue = crc.getValue();
		}
		catch (Exception e)
		{
			Tools.showException(e);
		}
		return crcValue;
	}

	private static byte[] getAddBytes(long oldCrc32, long newCrc32)
	{
		byte[] result = new byte[4];
		int[] index = new int[4];
		long[] crackXor = new long[4];
		long[] trueXor = new long[4];

		crackXor[3] = newCrc32 ^ 0x00000000ffffffffL;
		index[3] = getIndex(crackXor[3] >> 24);

		crackXor[2] = crackXor[3] ^ crc32Table[index[3]];
		index[2] = getIndex(crackXor[2] >> 16);

		crackXor[1] = crackXor[2] ^ (crc32Table[index[2]] >> 8);
		index[1] = getIndex(crackXor[1] >> 8);

		crackXor[0] = crackXor[1] ^ (crc32Table[index[1]] >> 16);
		index[0] = getIndex(crackXor[0]);

		trueXor[0] = oldCrc32 ^ 0x00000000ffffffffL;
		result[0] = getByte(trueXor[0], index[0]);

		trueXor[1] = getXor(trueXor[0], result[0]);
		result[1] = getByte(trueXor[1], index[1]);

		trueXor[2] = getXor(trueXor[1], result[1]);
		result[2] = getByte(trueXor[2], index[2]);

		trueXor[3] = getXor(trueXor[2], result[2]);
		result[3] = getByte(trueXor[3], index[3]);

		return result;
	}

	private static long getXor(long beforeXor, byte b)
	{
		return crc32Table[(int) ((beforeXor ^ b) & 0xff)] ^ (beforeXor >> 8);
	}

	private static int getIndex(long start)
	{
		for (int i = 0; i < crc32Table.length; i++)
		{
			if ((crc32Table[i] >> 24) == start)
			{
				return i;
			}
		}
		return 0;
	}

	private static byte getByte(long beforeXor, int index)
	{
		for (byte b = -128; b < 127; b++)
		{
			if (index == (int) ((beforeXor ^ b) & 0xff))
			{
				return b;
			}
		}
		return 0;
	}

}
