package com.example.hellolibs;

import android.content.SharedPreferences;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLUtils;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;

public class MyAssetManager {
    private AssetManager amgr;
    private SharedPreferences mSharedPreferences;
    MyAssetManager(AssetManager amgr, SharedPreferences sharedPreferences) {
        this.amgr = amgr;
        mSharedPreferences = sharedPreferences;
    }

    public String readFileContents(String path) {
        try {
            InputStream inputStream = amgr.open(path);
            ByteArrayOutputStream result = new ByteArrayOutputStream();
            byte[] buffer = new byte[1024];
            int length;
            while ((length = inputStream.read(buffer)) != -1) {
                result.write(buffer, 0, length);
            }
            return result.toString("UTF-8");
        } catch (IOException e) {
            return null;
        }
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

    public String loadUserData(String key) {
        return mSharedPreferences.getString(key, "");
    }

    public void saveUserData(String key, String value) {
        SharedPreferences.Editor editor = mSharedPreferences.edit();
        editor.putString(key, value);
        editor.apply();
    }

    public void closeBitmap(Bitmap bmp)
    {
        bmp.recycle();
    }
}
