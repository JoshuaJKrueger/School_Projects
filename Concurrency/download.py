'''
    Helper function for flags_(a, b, c).py
    Handles file downloading and saving with requests module
    Needs a flags directory to properly save files
'''
import requests

def download(country):
    '''Takes a country name as input and appends it to the url for download'''
    url = 'https://www.sciencekids.co.nz/images/pictures/flags680/'
    req = requests.get(f'{url}{country}.jpg')

    if req.status_code == requests.codes.ok:
        with open(f'flags/{country}.jpg', 'wb') as f:
            for data in req:
                f.write(data)
        return len(req.content)
