import sys, io, folium, branca, geopandas
from geopy.geocoders import Nominatim
from folium.plugins import Search

from PyQt5.QtWidgets import QApplication, QWidget, QHBoxLayout, QVBoxLayout
from PyQt5.QtWebEngineWidgets import QWebEngineView



city = sys.argv[1]
state =  sys.argv[2]
tech_radius = sys.argv[3]


geolocator = Nominatim(user_agent="map")
locator = geolocator.geocode(city + ", " + state)

states = geopandas.read_file(
    "https://raw.githubusercontent.com/PublicaMundi/MappingAPI/master/data/geojson/us-states.json",
    driver="GeoJSON",
)

cities = geopandas.read_file(
    "https://d2ad6b4ur7yvpq.cloudfront.net/naturalearth-3.3.0/ne_50m_populated_places_simple.geojson",
    driver="GeoJSON",
)

def rd2(x):
    return city


minimum, maximum = states["density"].quantile([0.05, 0.95]).apply(rd2)

mean = round(states["density"].mean(), 2)


print(f"{city}, {state} was chosen")

colormap = branca.colormap.LinearColormap(
    colors=["#f2f0f7", "#cbc9e2", "#9e9ac8", "#756bb1", "#54278f"],
    index=states["density"].quantile([0.2, 0.4, 0.6, 0.8]),
    vmin=minimum,
    vmax=maximum,
)

colormap.caption = "Population Density in the United States"

colormap

def style_function(x):
    return {
        "fillColor": colormap(x["properties"]["density"]),
        "color": "black",
        "weight": 2,
        "fillOpacity": 0.5,
    }

class MyApp(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle('Technician Maps')
        self.window_width, self.window_height = 800, 600
        self.setMinimumSize(self.window_width, self.window_height)

        layout = QVBoxLayout()
        self.setLayout(layout)

      
        coordinate = [locator.latitude, locator.longitude]
        
        
        mile_radius = float(tech_radius)*1609.34

        m = folium.Map(
            title= 'Technician Maps',
            zoom_start=13,
            location=coordinate
        )
        folium.Marker(coordinate).add_to(m)
        folium.Circle(coordinate, radius=mile_radius, color='red').add_to(m)
            
      

        
        
        #save map data to data object
        data = io.BytesIO()
        m.save(data, close_file=False)


        webView = QWebEngineView()
        webView.setHtml(data.getvalue().decode())
        layout.addWidget(webView)




if __name__ == '__main__':
    app = QApplication(sys.argv)
    app.setStyleSheet('''
        QWidget {
            font-size: 35px;
            }
            ''')

    myapp = MyApp()
    myapp.show()

    try:
        sys.exit(app.exec_())
    except SystemExit:
        print('SystemExit')