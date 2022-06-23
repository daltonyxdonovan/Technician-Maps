# Technician-Maps
## a GUI to display the names, phone numbers, and location of your technicians, with a map to boot!
## can be ran from anywhere, even a flash drive! completely portable! (for once)

===============================================================

# Usage
to get started, you'll have to install the vcredist file included with the zip (in the release section on the right side of this page!) to have the packages required to run c++ apps.
you'll also require the latest version of python to be installed and added to path. the installer can be found [here](https://www.python.org/ftp/python/3.10.5/python-3.10.5-amd64.exe) Adding it to path is as simple as **checking a box in the python installer**

## MAKE SURE YOU ADD PYTHON TO PATH WITH THE CHECKBOX IN THE INSTALLER

i've included my **csv converter** with the release, essentially you go to [google contacts](https://contacts.google.com/) and export your contacts as a **_Google CSV_** and
run the converter, choosing your csv you just downloaded. it will walk you through questions in the list, determining who is a tech, the tech's location and whatnot.
when this is done, you will be left with a **cleaned_contacts.csv**

the **Technician Maps** application is now ready to be ran, upon opening choose the cleaned_contacts.csv and there ya go! if a location and max driving radius is set, you can
click on the tech's card to pull up a map with a circle denoting their distance! easy as that!



# Some quirks to note:
-to search, you click the box and type your query. to _unsort_ the list, just search an empty box.

-don't maximize it, it gets pretty cursed. lol.


