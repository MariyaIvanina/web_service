import json
import random
import sys
import pymongo

def get_all_uids():
    myclient = pymongo.MongoClient("mongodb://localhost:27017/")
    mydb = myclient["testdb"]
    mycol = mydb["testcollection"]

    return [x['_id'] for x in mycol.find()]


def make_ammo(method, url, headers, case, body):
    """
    Create one bullet of specified type.
    Got that routine from yandex-tank documentation.
    """

    #http request w/o entity body template
    req_template = (
          "%s %s HTTP/1.1\r\n"
          "%s\r\n"
          "\r\n"
    )

    #http request with entity body template
    req_template_w_entity_body = (
          "%s %s HTTP/1.1\r\n"
          "%s\r\n"
          "Content-Length: %d\r\n"
          "\r\n"
          "%s\r\n"
    )

    if not body:
        req = req_template % (method, url, headers)
    else:
        req = req_template_w_entity_body % (method, url, headers, len(body), body)

    #phantom ammo template
    ammo_template = (
        "%d %s\n"
        "%s"
    )

    return ammo_template % (len(req), case, req)


def generate(count):
    put_headers = """Content-Type: application/json
Connection: close"""

    get_headers = """Connection: close"""
    
    user_ids = get_all_uids()
    count_put = 0
    count_get = 0
    for i in range(count):
        if random.randint(0,100) <= 30:
            body = json.dumps({
                 "eth_amount": i*1.3
                  })
            count_put += 1
            sys.stdout.write(make_ammo('PUT', "/user_wallet/{}".format(user_ids[i]), put_headers, 'put_requests', body))
        else:
            count_get += 1
            sys.stdout.write(make_ammo('GET', "/user_wallet/{}".format(user_ids[i]), get_headers, 'get_requests', ''))


if __name__ == "__main__":
    generate(300)
