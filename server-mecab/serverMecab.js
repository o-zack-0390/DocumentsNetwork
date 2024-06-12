import express from 'express';
import { wakati } from '@enjoyjs/node-mecab';

/* クライアント側から送信された文章を分かち書きするサーバー */

const app  = express();
const port = 4000;

app.use(express.json());

app.post('/analyze', async (req, res) => {
    const { text } = req.body;
    try {
        const result = await wakati(text);
        res.json({ result });
    } catch (error) {
        res.status(500).json({ error: error.message });
    }
});

app.listen(port, () => {
    console.log(`Server is running on http://localhost:${port}`);
});
