import GXClientFactory from "gxclient";
import { private_key, account_id, test_id } from './config';

let client = GXClientFactory.instance({
    keyProvider: private_key,
    account: account_id,
    network: "http://39.105.155.174:28090"
});

client.transfer(test_id, "", "0.5 GXC", true).then(trx => {
    console.log("transfer success");
}).catch(error => {
    debugger;
    if (error.code === 432) {
        console.log("you don't authorize identity!");
    }
    console.error(error);
});

async function main() {
    const trx = await client.getContractTable("windstest1");
    console.log(trx);
}

//main();