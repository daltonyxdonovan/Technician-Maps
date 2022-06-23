import sys
import subprocess

subprocess.check_call([sys.executable, '-m', 'pip', 'install', 'pipwin', 'pyqt5', 'folium', 'branca', 'qtwidgets', 'pyqtwebengine', 'geopy'])
reqs = subprocess.check_output([sys.executable, '-m', 'pip', 'freeze'])
installed_packages = [r.decode().split('==')[0] for r in reqs.split()]

subprocess.check_call([sys.executable, '-m', 'pipwin', 'install', 'gdal', 'fiona', 'geopandas'])
reqs = subprocess.check_output([sys.executable, '-m', 'pip', 'freeze'])
installed_packages = [r.decode().split('==')[0] for r in reqs.split()]