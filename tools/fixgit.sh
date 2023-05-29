#!/bin/bash
sudo chgrp -R sepharial .
sudo chmod -R g+rwX .
sudo find . -type d -exec chmod g+s '{}' +