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
  
## ⚒️使用方法
### [0]：動作環境と必要なツール
Windows10以上のWindows環境で動作します。  
これらのツールの事前インストールが必要です。  
| ツール名                                                        | 動作確認したver |
| --------------------------------------------------------------- | --------------- |
| [Docker](https://www.docker.com/ja-jp/products/docker-desktop/) | 28.0.1          |
| [gcc(C言語のコンパイラ)](https://www.mingw-w64.org/)            | 9.2.0           |
  
### [1]：リポジトリをクローン  
クローンした後にcdで入ってください。
```
git clone https://github.com/o-zack-0390/DocumentsNetwork.git
cd DocumentsNetwork
```  
  
### [2]：`server-cgi-bin/data`に検索対象の文章データを配置
`server-cgi-bin/data`に、検索対象の文章データを加工した後に手動で配置してください。ファイル名は必ず下記のリストと一致させてください。  
- `doc.txt`
- `lbl.txt`
- `uid.txt`
- `wakachi.txt`
- `wid.txt`
  
### [3]：バッチファイルを実行してApacheサーバーを構築＆起動
server-cgi-binのディレクトリに入ってください。
```
cd server-cgi-bin
```
`server-cgi-bin/1_setup.bat`を実行すると、Apacheサーバーを自動構築します。構築時に`server-cgi-bin/data`に配置したファイルを`Apache24/cgi-bin/data`に全てコピーします。
```
1_setup.bat
```
`server-cgi-bin/2_run.bat`を実行すると、構築したApacheサーバーを起動します。  
```
2_run.bat
```  

補足：`server-cgi-bin/1_setup.bat`と`server-cgi-bin/2_run.bat`は、エクスプローラー上からダブルクリックで実行することもできます。
  
### [4]：クライアントとMecabのDockerコンテナを起動
`settings`にあるDockerFileに基づき、クライアントとMecabのDockerコンテナを作成＆起動します。
```
docker compose up
```

### [5]：ブラウザで`http://localhost:3000/`を開きアプリを使用
[3]と[4]の項目にあるApacheサーバーとDockerコンテナを起動した状態で、`http://localhost:3000/`にアクセスするとアプリが使用できます。  

「Upload query document !」を押下して、検索クエリの文章データをアップロードすると、グラフによる検索結果の可視化ができます。  
詳細：[類似文書を検索してネットワーク図を生成する](https://or-expert.com/?p=3841)  
  
検索クエリの文章データの形式は以下の通りです。
- 拡張子は`.csv`。
- 最初の一行に検索クエリの文章データを改行コード無しで全部入れる。