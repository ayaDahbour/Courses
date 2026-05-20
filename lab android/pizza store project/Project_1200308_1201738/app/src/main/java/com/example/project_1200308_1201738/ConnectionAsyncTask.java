package com.example.project_1200308_1201738;

import android.app.Activity;
import android.os.AsyncTask;
import java.util.List;
public class ConnectionAsyncTask extends AsyncTask<String, String, String> {
    Activity activity;
    private ConnectionResultListener resultListener;

    public ConnectionAsyncTask(Activity activity) {
        this.activity = activity;
    }

    @Override
    protected void onPreExecute() {
        super.onPreExecute();
    }

    @Override
    protected String doInBackground(String... params) {
        String data = HttpManager.getData(params[0]);
        return data;
    }

    @Override
    protected void onPostExecute(String s) {
        super.onPostExecute(s);
        List<Pizza> P_list = PizzaJsonParser.getObjectFromJson(s);
        if (resultListener != null) {
            resultListener.onConnectionResult(P_list != null);
        }
    }

    public void setResultListener(ConnectionResultListener connectionResultListener) {
        this.resultListener = connectionResultListener;
    }

    public interface ConnectionResultListener {
        void onConnectionResult(boolean successful);
    }
}