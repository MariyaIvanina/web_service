import json
import random
import sys
import pymongo

def make_ammo(method, url, headers, case, body):

    req_template_w_entity_body = (
          "%s %s HTTP/1.1\r\n"
          "%s\r\n"
          "Content-Length: %d\r\n"
          "\r\n"
          "%s\r\n"
    )
    req = req_template_w_entity_body % (method, url, headers, len(body), body)
    """ makes phantom ammo """
    # uripost ammo template
    ammo_template = (
        "%d %s\n"
        "%s\n"
    )
    return ammo_template % (len(req), url, req)


def generate(count):
    headers = """Content-Type: application/json
Connection: close"""
    myclient = pymongo.MongoClient("mongodb://localhost:27017/")
    mydb = myclient["testdb"]
    mycol = mydb["testcollection"]

    user_ids = [x['_id'] for x in mycol.find()]
    
    for i in range(count):
        body = json.dumps({
            "eth_amount": i*1.3
        })
        sys.stdout.write(make_ammo('PUT', "/user_wallet/{}".format(user_ids[i]), headers, 'put_requests', body))


if __name__ == "__main__":
    generate(800)
