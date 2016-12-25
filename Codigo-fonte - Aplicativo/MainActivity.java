package com.example.pedro.projetogenulic;


import android.os.Handler;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.webkit.WebView;
import android.widget.ProgressBar;
import android.widget.TextView;


public class MainActivity extends AppCompatActivity {

    //Identificação dos componentes
    private int estadoProg = 0;
    private Handler handler = new Handler();

    TextView BemVindo;
    Button Azul;
    EditText End;
    ProgressBar Barra;
    int i;



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //Identificação dos componentes ou comandos que são dados no momento da abertura do app

        AlertDialog NomeQualquer;
        NomeQualquer = new AlertDialog.Builder(this).create();
        NomeQualquer.setTitle("Projeto Genulic");
        NomeQualquer.setMessage("Bem-Vindo! Insira o endereço de IP do módulo Wi-fi e teste os comandos");
        NomeQualquer.show();

        BemVindo = (TextView) findViewById(R.id.bemVindo);
        Azul = (Button) findViewById(R.id.dbotaoAzul);
        End = (EditText) findViewById(R.id.entrada);
        Barra = (ProgressBar) findViewById(R.id.barraProgresso);
    }

    ///***************** Função para desligar ******************************

    public void azul(View View) throws InterruptedException {

        BemVindo.setText("Aguarde...");

        WebView view = (WebView) this.findViewById(R.id.enviaDados);
        view.getSettings().setJavaScriptEnabled(true);

        String endereco = End.getText().toString() + "/lamp+des";

        for (i = 0; i < 5; i++)
        {
            view.loadUrl(endereco);
            Thread.sleep(500);
        }

        estadoProg = 0;
        new Thread(new Runnable() {
            public void run() {
                while (estadoProg < 100){
                    estadoProg += 1;
                    handler.post(new Runnable() {
                        @Override
                        public void run() {
                            Barra.setProgress(estadoProg);
                        }
                    });
                    try{
                        Thread.sleep(10);
                    }
                    catch(InterruptedException e){
                        e.printStackTrace();
                    }
                }
            }
        }).start();
        BemVindo.setText("Pronto!");
    }

    ///******************** Função para ligar ******************************

    public void lazul(View View) throws InterruptedException {

        BemVindo.setText("Aguarde...");

        WebView view = (WebView) this.findViewById(R.id.enviaDados);
        view.getSettings().setJavaScriptEnabled(true);

        String endereco = End.getText().toString() + "/lamp+lig";

        for (i = 0; i < 5; i++)
        {
            view.loadUrl(endereco);
            Thread.sleep(500);
        }
        estadoProg = 0;
        new Thread(new Runnable() {
            public void run() {
                while (estadoProg < 100){
                    estadoProg += 1;
                    handler.post(new Runnable() {
                        @Override
                        public void run() {
                            Barra.setProgress(estadoProg);
                        }
                    });
                    try{
                        Thread.sleep(20);
                    }
                    catch(InterruptedException e){
                        e.printStackTrace();
                    }
                }
                handler.post(new Runnable() {
                    @Override
                    public void run() {
                        BemVindo.setText("Pronto!");
                    }
                });
            }
        }).start();
    }

    //***************** Função para monitorar estado *************************

    public void recDados(View View) throws InterruptedException {

        BemVindo.setText("Aguarde...");

        WebView view = (WebView) this.findViewById(R.id.recebeDados);
        view.getSettings().setJavaScriptEnabled(true);

        String endereco = End.getText().toString();

        for (i = 0; i < 5; i++)
        {
            view.loadUrl(endereco);
            Thread.sleep(500);
        }
        estadoProg = 0;
        new Thread(new Runnable() {
            public void run() {
                while (estadoProg < 100){
                    estadoProg += 1;
                    handler.post(new Runnable() {
                        @Override
                        public void run() {
                            Barra.setProgress(estadoProg);
                        }
                    });
                    try{
                        Thread.sleep(30);
                    }
                    catch(InterruptedException e){
                        e.printStackTrace();
                    }
                }
                handler.post(new Runnable() {
                    @Override
                    public void run() {
                        BemVindo.setText("Pronto!");
                    }
                });
            }
        }).start();
    }
}

