package com.wwhuang.cloudcomm;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCallback;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattDescriptor;
import android.bluetooth.BluetoothGattService;
import android.bluetooth.BluetoothManager;
import android.bluetooth.BluetoothProfile;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.IBinder;
import android.os.Message;
import android.os.Parcel;
import android.os.PowerManager;
import android.os.Vibrator;
import android.util.Log;
import android.widget.TextView;

import com.google.common.primitives.Ints;

import org.apache.http.HttpResponse;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.ByteArrayEntity;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.protocol.HTTP;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;
import java.util.UUID;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;



public class CloudCommService extends Service {

    private BluetoothManager mBLManager;
    private BluetoothAdapter mBLAdapter;
    private BluetoothGatt failGatt;
    private AtomicBoolean urlSet;
    private HandlerThread BLThread;
    private Handler BLHandler;

    private HttpClient hc;
    private HttpPost hp;
    ArrayList<Byte> dataBuilder;

    private String CCUUIDSTRING = "00002000-0000-1000-8000-00805F9B34FB";
    private String CCDATAUUIDSTRING = "00002b00-0000-1000-8000-00805F9B34FB";
    private String CCMETAUUIDSTRING = "00002b01-0000-1000-8000-00805F9B34FB";
    private String CCREQUUIDSTRING = "00002b02-0000-1000-8000-00805F9B34FB";
    private String CCINCUUIDSTRING = "00002b03-0000-1000-8000-00805F9B34FB";
    private String CCREADYSTRING = "00002b04-0000-1000-8000-00805F9B34FB";
    private String CCACKSTRING = "00002b05-0000-1000-8000-00805F9B34FB";
    private String CccDescriptorUUIDString = "00002902-0000-1000-8000-00805F9B34FB";

    private UUID CCUUID = UUID.fromString(CCUUIDSTRING); // Cloudcomm Service UUID
    private UUID CCDATAUUID = UUID.fromString(CCDATAUUIDSTRING); // Peripheral uploads data on this Characteristic
    private UUID CCMETAUUID = UUID.fromString(CCMETAUUIDSTRING);
    private UUID CCREQUUID = UUID.fromString(CCREQUUIDSTRING); // Peripheral requests metadata using this Characteristic
    private UUID CCINCUUID = UUID.fromString(CCINCUUIDSTRING); // Phone sends data using this Characteristic
    private UUID CCREADYUUID = UUID.fromString(CCREADYSTRING); // Phone sends acks using this Characteristic
    private UUID CCACKUUID = UUID.fromString(CCACKSTRING);
    private UUID CCCDESCUUID = UUID.fromString(CccDescriptorUUIDString);

    private Handler failHandler;
    private Runnable failRunner;

    private int START_LE_SCAN = 0;
    private int CONNECT_BLE_PERIPHERAL = 1;
    private int DISCOVER_BLE_SERVICES = 2;
    private int WRITE_DATA_CHAR_DESC = 3;
    private int WRITE_REQ_DESC = 4;
    private int WRITE_META_DESC = 5;
    private int SEND_CC_READY = 6;
    private int READ_PERIPHERAL_DATA = 7;
    private int READ_PERIPHERAL_REQUEST = 8;
    private int SEND_CC_ACK = 9;
    private int SEND_PERIPHERAL_TIME = 10;
    private long failTime = 30000;

    final protected static char[] hexArray = "0123456789ABCDEF".toCharArray();
    public static String bytesToHex(byte[] bytes) {
        char[] hexChars = new char[bytes.length * 2];
        for ( int j = 0; j < bytes.length; j++ ) {
            int v = bytes[j] & 0xFF;
            hexChars[j * 2] = hexArray[v >>> 4];
            hexChars[j * 2 + 1] = hexArray[v & 0x0F];
        }
        return new String(hexChars);
    }

    private static String reversBytes(String s){
        StringBuilder b = new StringBuilder(s.length());
        String rs = new StringBuilder(s).reverse().toString();
        for(int i=0;i<rs.length();i+=2) {
            b.append(new StringBuilder(rs.substring(i, i+2)).reverse().toString());
        }
        return b.toString();
    }
    
    public CloudCommService() {

    }

    @Override
    public IBinder onBind(Intent intent) {
        // TODO: Return the communication channel to the service.
        throw new UnsupportedOperationException("Not yet implemented");
    }

    @Override
    public void onCreate() {
        failHandler = new Handler();
        failRunner = new Runnable() {
            @Override
            public void run() {
                Log.v("CC_FAILSAFE", "Failsafe timeout");
                failGatt.disconnect();
            }
        };

        mBLManager = (BluetoothManager) getSystemService(BLUETOOTH_SERVICE);
        mBLAdapter = mBLManager.getAdapter();
        urlSet = new AtomicBoolean(false);
        BLThread = new HandlerThread("CcBLHandlerThread");
        BLThread.start();
        BLHandler = new Handler(BLThread.getLooper(), ccBLHandlerCallback);
        dataBuilder = new ArrayList<Byte>();
    }


    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        if(!mBLAdapter.isEnabled()) {
            mBLAdapter.enable();
        }
        BLHandler.sendEmptyMessage(START_LE_SCAN);
        return Service.START_STICKY;
    }

    @Override
    public void onDestroy() {

    }

    Handler.Callback ccBLHandlerCallback = new Handler.Callback() {
       @Override
        public boolean handleMessage(Message msg) {
           BluetoothManager ccBLManager = (BluetoothManager) getSystemService(BLUETOOTH_SERVICE);
           BluetoothAdapter ccBLAdapter = ccBLManager.getAdapter();
           if(msg.what == START_LE_SCAN) {
               ccBLAdapter.startLeScan(mLeScanCallback);
           }
           else if(msg.what == CONNECT_BLE_PERIPHERAL) {
               ccBLAdapter.stopLeScan(mLeScanCallback);
               BluetoothDevice blDevice = (BluetoothDevice) msg.obj;
               blDevice.connectGatt(getApplicationContext(), false, mGattCallback);
           }
           else if(msg.what == DISCOVER_BLE_SERVICES) {
               BluetoothGatt mGatt = (BluetoothGatt) msg.obj;
               failHandler.postDelayed(failRunner, failTime);
               if(!mGatt.discoverServices()) {
                   Log.v("BLEServices", "failed");
               }
           }
           else if(msg.what == WRITE_DATA_CHAR_DESC) {
               BluetoothGatt gatt = (BluetoothGatt) msg.obj;
               BluetoothGattService ccService = gatt.getService(CCUUID);
               if (ccService != null) {
                   BluetoothGattCharacteristic ccData = ccService.getCharacteristic(CCDATAUUID);
                   List<BluetoothGattCharacteristic> explorer = ccService.getCharacteristics();
                   for(int i=0; i < explorer.size(); i++) {
                       Log.v("Explorer Char UUID", explorer.get(i).getUuid().toString());
                       Log.v("Explorer Char InstID", Integer.toString(explorer.get(i).getInstanceId()));
                   }
                   if (ccData != null) {
                       BluetoothGattDescriptor dataDesc = ccData.getDescriptor(CCCDESCUUID);
                       gatt.setCharacteristicNotification(ccData, true);
                       dataDesc.setValue(BluetoothGattDescriptor.ENABLE_INDICATION_VALUE);
                       failHandler.postDelayed(failRunner, failTime);
                       gatt.writeDescriptor(dataDesc);
                   }
               }
           }
           else if(msg.what == WRITE_REQ_DESC) {
               BluetoothGatt gatt = (BluetoothGatt) msg.obj;
               BluetoothGattService s = gatt.getService(CCUUID);
               BluetoothGattCharacteristic c = s.getCharacteristic(CCREQUUID);
               BluetoothGattDescriptor dataDesc = c.getDescriptor(CCCDESCUUID);
               gatt.setCharacteristicNotification(c, true);
               dataDesc.setValue(BluetoothGattDescriptor.ENABLE_INDICATION_VALUE);
               failHandler.postDelayed(failRunner, failTime);
               gatt.writeDescriptor(dataDesc);
           }
           else if(msg.what == WRITE_META_DESC) {
               BluetoothGatt gatt = (BluetoothGatt) msg.obj;
               BluetoothGattService s = gatt.getService(CCUUID);
               BluetoothGattCharacteristic c = s.getCharacteristic(CCMETAUUID);
               BluetoothGattDescriptor dataDesc = c.getDescriptor(CCCDESCUUID);
               gatt.setCharacteristicNotification(c, true);
               dataDesc.setValue(BluetoothGattDescriptor.ENABLE_INDICATION_VALUE);
               failHandler.postDelayed(failRunner, failTime);
               gatt.writeDescriptor(dataDesc);
           }
           else if(msg.what == SEND_CC_READY) {
               BluetoothGatt mGatt = (BluetoothGatt) msg.obj;
               BluetoothGattCharacteristic ccReadyChar = mGatt.getService(CCUUID).getCharacteristic(CCREADYUUID);
               if(ccReadyChar != null) {
                   ccReadyChar.setValue(0, BluetoothGattCharacteristic.FORMAT_UINT8, 0);
                   ccReadyChar.setWriteType(BluetoothGattCharacteristic.WRITE_TYPE_DEFAULT);
                   /*try {
                       Thread.sleep(600); // phones need breaks apparently?
                   } catch (InterruptedException e) {} */
                   failHandler.postDelayed(failRunner, failTime);
                   mGatt.writeCharacteristic(ccReadyChar);
               }
           }
           else if(msg.what == READ_PERIPHERAL_DATA) {
               BluetoothGatt mGatt = (BluetoothGatt) msg.obj;
               BluetoothGattCharacteristic ccDataChar = mGatt.getService(CCUUID).getCharacteristic(CCDATAUUID);
               failHandler.postDelayed(failRunner, failTime);
               mGatt.readCharacteristic(ccDataChar);
           }
           else if(msg.what == SEND_CC_ACK) {
               BluetoothGatt mGatt = (BluetoothGatt) msg.obj;
               BluetoothGattCharacteristic ccReadyChar = mGatt.getService(CCUUID).getCharacteristic(CCREADYUUID);
               ccReadyChar.setValue(msg.arg1, BluetoothGattCharacteristic.FORMAT_UINT8, 0);
               Log.v("ccReadyCharVal", Integer.toString(msg.arg1));
               ccReadyChar.setWriteType(BluetoothGattCharacteristic.WRITE_TYPE_DEFAULT);
               /* try { Thread.sleep(100); } catch (InterruptedException e) {} */
               failHandler.postDelayed(failRunner, failTime);
               if(!(mGatt.writeCharacteristic(ccReadyChar))) {
                   Log.v("BleCharWrite", "failed");
               }
           }
           else if(msg.what == READ_PERIPHERAL_REQUEST) {
               BluetoothGatt mGatt = (BluetoothGatt) msg.obj;
               BluetoothGattCharacteristic ccReqChar = mGatt.getService(CCUUID).getCharacteristic(CCREQUUID);
               Log.v("BleReq", "Time to Read!");
               failHandler.postDelayed(failRunner, failTime);
               mGatt.readCharacteristic(ccReqChar);
           }
           else if(msg.what == SEND_PERIPHERAL_TIME) {
               BluetoothGatt mGatt = (BluetoothGatt) msg.obj;
               BluetoothGattCharacteristic ccIncChar = mGatt.getService(CCUUID).getCharacteristic(CCINCUUID);
               byte[] senddata = new byte[5];
               senddata[0] = 0;
               int ctime = (int)(System.currentTimeMillis() / 1000L);
               senddata[1] = (byte)(ctime >>> 24);
               senddata[2] = (byte)(ctime >>> 16);
               senddata[3] = (byte)(ctime >>> 8);
               senddata[4] = (byte)(ctime);
               ccIncChar.setValue(senddata);
               Log.v("ccIngCharVal", Integer.toString(ctime));
               ccIncChar.setWriteType(BluetoothGattCharacteristic.WRITE_TYPE_DEFAULT);
               // try { Thread.sleep(100); } catch (InterruptedException e) {}
               failHandler.postDelayed(failRunner, failTime);
               if(!(mGatt.writeCharacteristic(ccIncChar))) {
                   Log.v("BleIncCharWrite", "failed");
               }
           }
           return true;
       }
    };


    private final BluetoothAdapter.LeScanCallback mLeScanCallback = new BluetoothAdapter.LeScanCallback() {
        // Find a cloudcomm device, connect to it, and stop scanning for other devices
        @Override
        public void onLeScan(final BluetoothDevice bluetoothDevice, int i, byte[] bytes) {
            String dname = bluetoothDevice.getName();
            if(dname != null && dname.matches("Cloudcomm")) {
                Message m = Message.obtain(BLHandler, CONNECT_BLE_PERIPHERAL, bluetoothDevice);
                BLHandler.sendMessage(m);
            }
        }
    };

    private final BluetoothGattCallback mGattCallback = new BluetoothGattCallback() {
            @Override
            public void onConnectionStateChange(BluetoothGatt gatt, int status, int newState) {
                super.onConnectionStateChange(gatt, status, newState);
                if (newState == BluetoothProfile.STATE_CONNECTED) {
                    failGatt = gatt;
                    Message m = Message.obtain(BLHandler, DISCOVER_BLE_SERVICES, gatt);
                    BLHandler.sendMessage(m);
                }
                else if (newState == BluetoothProfile.STATE_DISCONNECTED) {
                    gatt.close();
                    if (!mBLAdapter.isEnabled()) {
                        mBLAdapter.enable();
                    }
                    dataBuilder.clear();
                    urlSet.set(false);
                    BLHandler.sendEmptyMessage(START_LE_SCAN);
                }
            }

            @Override
            public void onServicesDiscovered(BluetoothGatt gatt, int status) {
                super.onServicesDiscovered(gatt, status);
                if (status == BluetoothGatt.GATT_SUCCESS) {
                    failHandler.removeCallbacks(failRunner);
                    Message m = Message.obtain(BLHandler, WRITE_DATA_CHAR_DESC, gatt);
                    BLHandler.sendMessage(m);
                }
            }

            @Override
            public void onCharacteristicRead(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic, int status) {
                super.onCharacteristicRead(gatt, characteristic, status);
                if (characteristic.getUuid().equals(CCDATAUUID)) {
                    failHandler.removeCallbacks(failRunner);
                    byte[] cData = characteristic.getValue();
                    int seq_num = cData[0];
                    if(seq_num < 0) {
                        seq_num += 256;
                    }

                    for(int i=1;i<cData.length;i++) {
                        dataBuilder.add(cData[i]);
                    }

                    if(seq_num == 255) {
                        Log.v("ccDataSend", "cloudsend");
                        byte[] finalData = new byte[dataBuilder.size()];
                        for (int i = 0; i < finalData.length; i++) {
                            finalData[i] = (byte) dataBuilder.get(i);
                        }
                        String mUrl = new String("boo");
                        if(urlSet.get() == false) {
                            try {
                                mUrl = new String(finalData, "ascii");
                            } catch (UnsupportedEncodingException e) {

                            }
                            Log.v("mUrl", mUrl);
                            hc = new DefaultHttpClient();
                            hp = new HttpPost(mUrl);
                            urlSet.set(true);
                        } else {
                            String fd = bytesToHex(finalData);
                            if(fd.length() == 64) {
                                JSONObject j = new JSONObject();

                                long version_num = Long.parseLong(reversBytes(fd.substring(0, 4)), 16);
                                long rx_id = Long.parseLong(reversBytes(fd.substring(4, 8)), 16);
                                long tx_id = Long.parseLong(reversBytes(fd.substring(8, 12)), 16);
                                long range_dt = Long.parseLong(reversBytes(fd.substring(12, 20)), 16);
                                long ul_dt = Long.parseLong(reversBytes(fd.substring(20, 28)), 16);
                                long m_time_confidence = Long.parseLong(reversBytes(fd.substring(28, 30)), 16);
                                long m_unixtime = Long.parseLong(reversBytes(fd.substring(30, 38)), 16);
                                long failed_rx_count = Long.parseLong(reversBytes(fd.substring(38, 46)), 16);
                                long tx_unixtime = Long.parseLong(reversBytes(fd.substring(46, 54)), 16);
                                long tx_time_confidence = Long.parseLong(reversBytes(fd.substring(54, 56)), 16);
                                long tx_ul_rf_dt = Long.parseLong(reversBytes(fd.substring(56, 64)), 16);

                                try {
                                    j.put("version_num", version_num);
                                    j.put("rx_id", rx_id);
                                    j.put("tx_id", tx_id);
                                    j.put("range_dt", range_dt);
                                    j.put("ul_dt", ul_dt);
                                    j.put("m_time_confidence", m_time_confidence);
                                    j.put("m_unixtime", m_unixtime);
                                    j.put("failed_rx_count", failed_rx_count);
                                    j.put("tx_unixtime", tx_unixtime);
                                    j.put("tx_time_confidence", tx_time_confidence);
                                    j.put("tx_ul_rf_dt", tx_ul_rf_dt);
                                } catch (JSONException e) {
                                    Log.v("ccJsonError", "fck");
                                    e.printStackTrace();
                                }
                                Log.v("ccRawData", fd);
                                Log.v("ccDataString", j.toString());
                                try {
                                    hp.setEntity(new StringEntity(j.toString()));
                                    HttpResponse r = hc.execute(hp);
                                    r.getEntity().consumeContent();
                                } catch (ClientProtocolException e) {
                                    Log.v("ccHpException", "Client protocol exception");
                                } catch (IOException e) {
                                    Log.v("ccHpException", "IO exception");
                                }
                            }
                            else {
                                Log.v("ccStringError", "Length: " + fd.length());
                                Log.v("ccStringError2", fd);
                            }

                        }
                        dataBuilder.clear();
                    }
                    Log.v("seqnum", Integer.toString(seq_num));
                    Log.v("cData", bytesToHex(cData));
                    Message m = Message.obtain(BLHandler, SEND_CC_ACK, seq_num, 0, gatt);
                    BLHandler.sendMessage(m);
                }
                else if (characteristic.getUuid().equals(CCMETAUUID)) {}
                else if (characteristic.getUuid().equals(CCREQUUID)) {
                    failHandler.removeCallbacks(failRunner);
                    byte[] cData = characteristic.getValue();
                    int reqnum = cData[0];
                    Log.v("CCREQ", Integer.toString(reqnum));
                    if(reqnum == 0) {
                        Message m = Message.obtain(BLHandler, SEND_PERIPHERAL_TIME, 0, 0, gatt);
                        BLHandler.sendMessage(m);
                    }
                }
            }

            @Override
            public void onCharacteristicWrite(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic, int status) {
                super.onCharacteristicWrite(gatt, characteristic, status);
                if (characteristic.getUuid().equals(CCREADYUUID)) {
                    if(status != BluetoothGatt.GATT_SUCCESS) {
                        Log.v("ccOnCharWrite", "CCREADY write failed");
                    }
                    else {
                        failHandler.removeCallbacks(failRunner);
                        Log.v("ccOnCharWrite", "CCREADY WRITE SUCESS");
                    }
                }
                else if(characteristic.getUuid().equals(CCINCUUID)) {
                    if(status != BluetoothGatt.GATT_SUCCESS) {
                        Log.v("ccOnCharWrite", "CINC write failed");
                    }
                    else {
                        failHandler.removeCallbacks(failRunner);
                        Log.v("ccOnCharWrite", "CINC WRITE SUCESS");
                    }
                }
                else if (characteristic.getUuid().equals(CCACKUUID)) {}
            }

            @Override
            public void onCharacteristicChanged(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic) {
                super.onCharacteristicChanged(gatt, characteristic);
                if(characteristic.getUuid().equals(CCDATAUUID)) {
                    Message m = Message.obtain(BLHandler, READ_PERIPHERAL_DATA, gatt);
                    BLHandler.sendMessage(m);
                }
                else if(characteristic.getUuid().equals(CCREQUUID)) {
                    Message m = Message.obtain(BLHandler, READ_PERIPHERAL_REQUEST, gatt);
                    BLHandler.sendMessage(m);
                }
            }

            @Override
            public void onDescriptorRead(BluetoothGatt gatt, BluetoothGattDescriptor descriptor, int status) {
                super.onDescriptorRead(gatt, descriptor, status);
            }

            @Override
            public void onDescriptorWrite(BluetoothGatt gatt, final BluetoothGattDescriptor descriptor, int status) {
                super.onDescriptorWrite(gatt, descriptor, status);
                if (descriptor.getUuid().equals(CCCDESCUUID) && status == BluetoothGatt.GATT_SUCCESS) {
                    failHandler.removeCallbacks(failRunner);
                    if (descriptor.getCharacteristic().getUuid().equals(CCDATAUUID)) {
                        Message m = Message.obtain(BLHandler, WRITE_REQ_DESC, gatt);
                        BLHandler.sendMessage(m);
                    }
                    else if (descriptor.getCharacteristic().getUuid().equals(CCREQUUID)) {
                        Message m = Message.obtain(BLHandler, WRITE_META_DESC, gatt);
                        BLHandler.sendMessage(m);
                    }
                    else if (descriptor.getCharacteristic().getUuid().equals(CCMETAUUID)) {
                        Message m = Message.obtain(BLHandler, SEND_CC_READY, gatt);
                        BLHandler.sendMessage(m);
                    }
                }
            }

            @Override
            public void onReliableWriteCompleted(BluetoothGatt gatt, int status) {
                super.onReliableWriteCompleted(gatt, status);
            }
    };

}
