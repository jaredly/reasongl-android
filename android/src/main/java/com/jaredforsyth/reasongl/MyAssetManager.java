package com.jaredforsyth.reasongl;

import static android.content.Context.MODE_PRIVATE;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.SharedPreferences;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.opengl.GLUtils;
import android.os.Handler;
import android.util.Base64;
import android.util.Log;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.*;

import java.net.Socket;
import java.net.UnknownHostException;
import java.net.ConnectException;

public class MyAssetManager {
    private final AssetManager amgr;
    private final Context mContext;
    private final SharedPreferences mSharedPreferences;
    private final ReasonGLView view;
    private boolean mShowingDialog = false;
    public double pixelDensity;

    private boolean isListening = false;

    MyAssetManager(Context context, float pixelDensity, final ReasonGLView view) {
        this.view = view;
        this.amgr = context.getAssets();
        mContext = context;
        this.pixelDensity = pixelDensity;
        mSharedPreferences = context.getSharedPreferences("reasongl", MODE_PRIVATE);
    }

    public void startHotReloading(final String host, final String baseFile) {
        if (isListening) {
            return;
        };
        isListening = true;
        Thread thread = new Thread(new Runnable(){
        @Override
        public void run(){

            Socket socket = null;
            String response = "";
            // "192.168.0.100"
            try {
                socket = new Socket(host, 8090);
            } catch (UnknownHostException e) {
                Log.e("reasongl", "Bad host: " + e);
                // view.onHotReload("/sdcard/hot.cma");
                return;
            } catch (ConnectException e) {
                Log.e("reasongl", "No hot reload server running: " + e);
                // view.onHotReload("/sdcard/hot.cma");
                return;
            } catch (IOException e) {
                Log.e("reasongl", "IO exception creating socket: " + e);
                // view.onHotReload("/sdcard/hot.cma");
                return;
            }

            try {
                String req = "android:" + baseFile + "\n";

                OutputStream out = socket.getOutputStream();
                out.write(req.getBytes());
                out.flush();
            } catch (IOException e) {
                Log.e("reasongl", "Failed to send who I am");
                return;
            }

            // if (true) {
            //     view.onHotReload("/sdcard/hot.cma");
            //     return;
            // }

            try {
                Log.e("reasongl", "SOCKET C");
                int bytesRead;
                InputStream inputStream = socket.getInputStream();

                ByteArrayOutputStream countStream = new ByteArrayOutputStream(20);
                byte[] countBuffer = new byte[20];

                while (true) {
                    Log.e("reasongl", "Starting a read");
                    byte b;
                    String collector = "";
                    while (true) {
                        b = (byte)inputStream.read();
                        if ((char)b == ':') {
                            break;
                        }
                        collector += ((char)b);
                    }
                    int size = Integer.parseInt(collector);

                    ByteArrayOutputStream fileStream = new ByteArrayOutputStream(size);
                    byte[] fileBuffer = new byte[1024];

                    int total = 0;
                    while (total < size) {
                        final int read = inputStream.read(fileBuffer);
                        if (read == -1) {
                            Log.e("reasongl", "Socket closed while reading body");
                            return;
                        }
                        fileStream.write(fileBuffer, 0, read);
                        total += read;
                    }
                    Log.e("reasongl", "FINISHED READING" + size);
                    File outputDir = mContext.getCacheDir();
                    File outputFile = File.createTempFile("hot", "cma", outputDir);
                    byte[] bytes = fileStream.toByteArray();
                    FileOutputStream f = new FileOutputStream(outputFile);
                    f.write(bytes);
                    f.close();
                    // view.onHotReload("/sdcard/hot.cma");
                    Log.e("reasongl", outputFile.getPath());
                    view.onHotReload(outputFile.getPath());
                    // Log.e("reasongl", "AAAAAAAAAAAAAAAA Got a reponse:\n" + response);
                }
            } catch (IOException e) {
                Log.e("reasongl", "IO Exception" + e);
                return;
            }

            /*
            * notice: inputStream.read() will block if no data return
            */
            // while ((bytesRead = inputStream.read(buffer)) != -1) {

            //     ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream(1024);
            //     byte[] buffer = new byte[1024];

            //     Log.e("reasongl", "SOCKET E");
            //     byteArrayOutputStream.write(buffer, 0, bytesRead);
            //     response += byteArrayOutputStream.toString("UTF-8");
            // }
            // Log.e("reasongl", "SOCKET E");

            // } catch (IOException e) {
            //     // TODO Auto-generated catch block
            //     e.printStackTrace();
            //     response = "IOException: " + e.toString();
            // } finally {
            //     if (socket != null) {
            //         try {
            //             socket.close();
            //         } catch (IOException e) {
            //             // TODO Auto-generated catch block
            //             e.printStackTrace();
            //         }
            //     }
            // }
        }
        });
        thread.start();
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

    public void showAlert(final String title, final String message) {
        if (mShowingDialog) {
            Log.e("reasongl", "Not showing alert, because one is already open. " + title + " : " + message);
            return;
        }
        mShowingDialog = true;

        Handler mainHandler = new Handler(mContext.getMainLooper());

        Runnable myRunnable = new Runnable() {
            @Override
            public void run() {
                new AlertDialog.Builder(mContext)
                        .setMessage(message)
                        .setTitle(title)
                        .setPositiveButton("Ok", new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialogInterface, int i) {
                                mShowingDialog = false;
                            }
                        })
                        .setIcon(android.R.drawable.ic_dialog_alert)
                        .show();

            } // This is your code
        };
        mainHandler.post(myRunnable);

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

    public void fillTextureWithColor(int target, int level, int red, int green, int blue, int alpha) {
        Bitmap image = Bitmap.createBitmap(1, 1, Bitmap.Config.ARGB_8888);
        image.eraseColor(Color.argb(alpha, red, green, blue));
        GLUtils.texImage2D(target, level, image, 0);
    }

    public int getBitmapWidth(Bitmap bmp) { return bmp.getWidth(); }
    public int getBitmapHeight(Bitmap bmp) { return bmp.getHeight(); }

    public byte[] loadUserData(String key) {
        String data = mSharedPreferences.getString(key, "");
        Log.d("Got Data", data);
        if (data.isEmpty()) {
            return null;
        } else {
            try {
                byte[] value = Base64.decode(data, Base64.DEFAULT);
                return value;
            } catch (Exception e) {
                return null;
            }
        }
    }

    public void saveUserData(String key, byte[] value) {
        SharedPreferences.Editor editor = mSharedPreferences.edit();
        String data = Base64.encodeToString(value, Base64.DEFAULT);
        Log.d("Saving Data", data);
        editor.putString(key, data);
        editor.apply();
    }

    public void closeBitmap(Bitmap bmp)
    {
        bmp.recycle();
    }
}
