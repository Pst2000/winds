'use strict';

import GXClientFactory from 'gxclient';
import { private_key, account_id, test_id } from './config';

let client = GXClientFactory.instance({
    keyProvider: private_key,
    account: account_id,
    network: 'http://39.105.155.174:28090'
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
    return new Promise((resolve, reject) => {
        client.getBlock(blockID).then(block => {
            resolve(parseBlock(block));
        });
    });
}

async function main() {
    const pos = await getBlockHeight();
    console.log(pos);

    const input = [13045795, 13045212, 13045401, 13045302];
    const obj = await Promise.all(input.map(generateQuery));
    const list = obj.reduce((x, y) => x.concat(y));
    list.forEach(x => {
        console.log(x.account + ' ' + x.data);
    });

    return;
    const trx = await client.getContractTable('windstest1');
    console.log(trx);
}

main();