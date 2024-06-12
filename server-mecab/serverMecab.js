import express from 'express';
import MeCab from 'mecab-async';

/* ==== Begin Function ==== */
function parseAsync(text)
{
    /* わかち書き文章を生成する関数 */
    return new Promise((resolve, reject) => {
        mecab.wakachi(text, (err, result) => {
            if (err) {
                reject(err);
            } else {
                resolve(result);
            }
        });
    });
}
/* ==== End Function ==== */

/* ==== Begin Server Setting ==== */
const app           = express();
const mecab         = new MeCab();
const port          = process.env.PORT || 4000;
const allowedOrigin = process.env.ALLOWED_ORIGIN || 'http://localhost:3000';

app.use(express.json());
app.use((req, res, next) => {
    res.setHeader('Access-Control-Allow-Origin', allowedOrigin);
    res.setHeader('Access-Control-Allow-Methods', '*');
    res.setHeader('Access-Control-Allow-Headers', '*');
    res.setHeader('Access-Control-Allow-Credentials', 'true');
    if (req.method === 'OPTIONS') {
        return res.sendStatus(204); // No Content
    }
    next();
});
/* ==== End Server Setting ==== */

/* ==== Begin POST Server ==== */
app.post('/', async (req, res) => {
    const text = req.body.text;
    try {
        const words = await parseAsync(text);
        res.json({ wakachiSentence: words.join(' ') });
    } catch (err) {
        console.error(err);
        res.status(500).send('わかち書き中にエラーが発生しました');
    }
});
/* ==== End POST Server ==== */

/* ==== Begin PORT Setting ==== */
app.listen(port, () => {
    console.log(`Express server listening on port ${port}`);
});
/* ==== End PORT Setting ==== */
