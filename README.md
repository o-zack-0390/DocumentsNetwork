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
`server-cgi-bin/data`に、検索対象の文章データを加工した後に手動で配置してください。ファイル名は必ず下記のリストと一致。  
- `doc.txt`
- `lbl.txt`
- `uid.txt`
- `wakachi.txt`
- `wid.txt`
  
### [3]：バッチファイルを実行してApacheサーバーを構築＆起動
`server-cgi-bin/1_setup.bat`を実行すると自動でApacheサーバーが構築されます。`server-cgi-bin/data`にあるファイルを生成される`Apache24`ディレクトリに全てコピーします。
```
server-cgi-bin\1_setup.bat
```
`server-cgi-bin/2_run.bat`を実行すると、構築したApacheサーバーを起動します。  
```
server-cgi-bin\2_run.bat
```  
  
### [4]：クライアントとMecabのDockerコンテナを起動
`settings`にあるDockerFileに基づき、クライアントとMecabのDockerコンテナを作成＆起動します。
```
docker compose up
```

### [5]：ブラウザで`http://localhost:3000/`を開きアプリを使用
`http://localhost:3000/`にアクセスすることでアプリが使用できます。「Upload query document !」を押下して、検索クエリの文章データをアップロードした後に操作ができます。  
  
検索クエリの文章データの形式は以下の通りです。
- 拡張子は`.csv`。
- 最初の一行に検索クエリの文章データを改行コード無しで全部入れる。