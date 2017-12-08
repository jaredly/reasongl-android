package com.example.hellolibs;

import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLUtils;

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

    public void texImage2DWithBitmap(int target, int level, Bitmap bitmap, int border) {
        GLUtils.texImage2D(target, level, bitmap, border);
    }

    public int getBitmapWidth(Bitmap bmp) { return bmp.getWidth(); }
    public int getBitmapHeight(Bitmap bmp) { return bmp.getHeight(); }

//    public void getBitmapPixels(Bitmap bmp, byte[] pixels)
//    {
//        int bytes = bmp.getByteCount();
//        ByteBuffer buffer = ByteBuffer.allocate(bytes); //Create a new buffer
//        bmp.copyPixelsToBuffer(buffer); //Move the byte data to the buffer
//
//        byte[] array = buffer.array();
//        System.arraycopy(array, 0, pixels, 0, bytes);
//    }

    public void closeBitmap(Bitmap bmp)
    {
        bmp.recycle();
    }
}
