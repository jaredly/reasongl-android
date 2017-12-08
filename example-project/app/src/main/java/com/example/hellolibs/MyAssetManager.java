package com.example.hellolibs;

import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import java.nio.ByteBuffer;

public class MyAssetManager {
    private AssetManager amgr;
    MyAssetManager(AssetManager amgr) {
        this.amgr = amgr;
    }

    public Bitmap openBitmap(String path)
    {
        try {
            return BitmapFactory.decodeStream(amgr.open(path));
        } catch (Exception e) {
            return null;
        }
    }

    public int getBitmapWidth(Bitmap bmp) { return bmp.getWidth(); }
    public int getBitmapHeight(Bitmap bmp) { return bmp.getHeight(); }

    public void getBitmapPixels(Bitmap bmp, byte[] pixels)
    {
        int bytes = bmp.getByteCount();
        ByteBuffer buffer = ByteBuffer.allocate(bytes); //Create a new buffer
        bmp.copyPixelsToBuffer(buffer); //Move the byte data to the buffer

        byte[] array = buffer.array();
        System.arraycopy(array, 0, pixels, 0, bytes);
//        int channels = 4;
//        int w = bmp.getWidth();
//        int h = bmp.getHeight();
//        int[] array = new int[w * h];
//        bmp.getPixels(array, 0, w, 0, 0, w, h);
//        for (int y = 0; y < h; y++) {
//            for (int x=0; x<w; x++) {
//                int i = y * w * channels + x;
//                int pixel = bmp.getPixel(x, y);
//                pixels[i + 0] = (byte)Color.red(pixel);
//                pixels[i + 1] = (byte)Color.green(pixel);
//                pixels[i + 2] = (byte)Color.blue(pixel);
//                pixels[i + 3] = (byte)Color.alpha(pixel);
//            }
//        }
    }

    public void closeBitmap(Bitmap bmp)
    {
        bmp.recycle();
    }
}
