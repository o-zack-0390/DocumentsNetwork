# 📄 DocumentsNetwork  

**DocumentsNetwork** は、クエリ文書の類似文書を検索し、ネットワーク図として可視化するアプリケーションです。  


## 🎯 機能概要  
- クエリに基づいて類似文書を検索  
- 文書間の関連性をネットワークとして可視化  
- 直感的なインターフェースで関係性を把握  

## 🖼️ 画面イメージ  

<p align="center">
  <img width="852" alt="DocumentsNetwork Screenshot" src="https://github.com/user-attachments/assets/90f927b4-236b-4c8e-836f-0d89f81fcbb8">
</p>

## 📚 解説記事
🔗 [類似文書を検索してネットワーク図を生成する](https://or-expert.com/?p=3841)  

## 🏅 受賞歴
- 第８７回 情報処理学会 全国大会 学生奨励賞  
  
<br>  
  
## ⚒️使用方法
### [0]：事前インストールが必要なツール
Dockerの事前インストールが必要です。  
| ツール名                                                        | 動作確認したver |
| --------------------------------------------------------------- | --------------- |
| [Docker](https://www.docker.com/ja-jp/products/docker-desktop/) | 28.0.1          |
  
<br>  
  
### [1]：リポジトリをクローン  
クローンした後にcdで入ってください。
```
git clone https://github.com/o-zack-0390/DocumentsNetwork.git
cd DocumentsNetwork
```  
  
<br>  
  
### [2]：`/0_data`に検索対象の文章データを配置
検索対象の文章データを作成して、`/0_data`に配置してください。**ファイル名は必ず下記のリストのものと完全一致。**
- `/0_data/doc.txt`
- `/0_data/wakachi.txt`
- `/0_data/uid.txt`

#### `/0_data/doc.txt`  
一行に一つの文章データを配置したもの。  
```
サンプルの文章Aです。
サンプルの文章Bです。
サンプルの文章Cです。
```  
####  `/0_data/wakachi.txt`  
doc.txtの文章を分かち書きしたもの(半角スペース区切り)。
```
サンプル の 文章A です 。
サンプル の 文章B です 。
サンプル の 文章C です 。
```  
####  `/0_data/uid.txt`  
文章のカテゴリ番号とファイル名を行毎に配置したもの。  
```
1 documentA.txt
2 documentB.txt
3 documentC.txt
```  
  
<br>  
  
### [3_A]：Dockerを使用して各サーバーを起動(簡単・ストレージ消費大)
`settings`にあるDockerFileに基づき、Dockerコンテナのビルド＆各サーバーの起動をします。ローカルにある`0_data`のディレクトリを、コンテナ内のディレクトリにバインドします。  
```
docker compose up
```  

  
※DockerはPCのストレージの消費量が大きいです。ストレージ消費を少なく済ませたい場合は、3_Bの方に書いてある方法を使ってください。  
<br>  
  
### [3_B]：手動で各サーバーを起動(Windows限定・ストレージ消費小)
Dockerを使用せずに、Windowsのローカル環境で動作させたい場合は、以下の折りたたみの中に書いてあることを実行して、アプリを起動してください。Windows10以上のWindows環境での動作を確認しています。node.jsとgccの事前インストールが必要です。  
| ツール名                                                        | 動作確認したver |
| --------------------------------------------------------------- | --------------- |
| [node.js](https://nodejs.org/ja)            | 23.7.0           |
| [gcc(C言語のコンパイラ)](https://www.mingw-w64.org/)            | 9.2.0           |
  
<br>  
  
<details>
<summary>ここをクリックしてアプリの起動方法を表示</summary>  

**「client」と「mecab」と「Apache」のサーバーをそれぞれ起動します。**  
**これら3つのサーバーを同時に起動している時にアプリが使用できます。**


### ◎：clientのサーバーを起動
```
cd client
npm install
npm start
```
  
### ◎：mecabのサーバーを起動
```
cd server-mecab
npm install
npm start
```
  
### ◎：Apacheのサーバーを起動
server-cgi-binのディレクトリに入ってください。
```
cd server-cgi-bin
```
`server-cgi-bin/1_setup.bat`を実行すると、Apacheサーバーを自動構築します。構築時に`server-cgi-bin`に配置したファイルを`Apache24/cgi-bin`に全てコピーします。
```
1_setup.bat
```
`server-cgi-bin/2_run.bat`を実行すると、構築したApacheサーバーを起動します。  
```
2_run.bat
```  

補足：`server-cgi-bin/1_setup.bat`と`server-cgi-bin/2_run.bat`は、エクスプローラー上からダブルクリックで実行することもできます。
</details>
  
<br>  
  
### [4]：ブラウザで`http://localhost:3000/`を開きアプリを使用
[3_A]か[3_B]の項目にある内容を実行し、各サーバーを起動した状態で、`http://localhost:3000/`にアクセスするとアプリが使用できます。  

「Upload query document !」を押下して、検索クエリの文章データをアップロードすると、グラフによる検索結果の可視化ができます。詳細：[類似文書を検索してネットワーク図を生成する](https://or-expert.com/?p=3841)  
  
<br>  
  
検索クエリの文章データの形式は以下の通りです。
- 拡張子は何でもOK。
- 最初の一行に検索クエリの文章データを改行コード無しで全部入れる。
```
検索したい文章のサンプルです。何か適当な内容。
```