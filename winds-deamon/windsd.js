'use strict';

import GXClientFactory from 'gxclient';
import { private_key, account_id, test_id } from './config';

let client = GXClientFactory.instance({
    keyProvider: private_key,
    account: account_id,
    network: 'https://testnet.gxchain.org'
});

async function getBlockHeight() {
    const prop = await client.getDynamicGlobalProperties();
    return prop.head_block_number;
}

async function parseBlock(block) {
    const result = [];
    block.transactions.forEach(x => {
        x.operations.forEach(op => {
            const {account, method_name, data} = op[1];
            if (method_name != 'push') return;
            result.push({account, data: Buffer.from(data, 'hex').toString('utf8', 1)});
        });
    });
    return result;
}

function generateQuery(blockID) {
    if (typeof blockID == 'number') {
        return Promise.all([ new Promise((resolve, reject) => {
            client.getBlock(blockID).then(block => {
                resolve(parseBlock(block));
            });
        }) ]);
    } else {
        return Promise.all(blockID.map(generateQuery));
    }
}

async function doQuery(blockID) {
    const obj = await generateQuery(prevBlock);
    return obj.reduce((x, y) => x.concat(y));
}

function sleep(delay) {
    return new Promise((resolve, reject) => {
        setTimeout(resolve, delay);
    })
}

let prevBlock, currBlock = 0;

async function enumBlock() {
    const pos = await getBlockHeight();
    if (currBlock != pos) {
        prevBlock = currBlock; currBlock = pos;
        if (prevBlock != 0) {
            console.log(`parsing the block #${prevBlock}`);
            const list = await doQuery(prevBlock);
            list.forEach(x => {
                console.log(x.account + ' ' + x.data);
            });
        }
    }
    await sleep(1200);
    enumBlock();
}

enumBlock();