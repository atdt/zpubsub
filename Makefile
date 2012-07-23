SRC_DIR = src
REV 	= $(shell git rev-parse HEAD | cut -c1-10)

.PHONY: project_src

project_src:
	$(MAKE) -C $(SRC_DIR)

clean:
	$(MAKE) -C $(SRC_DIR) clean

install:
	$(MAKE) -C $(SRC_DIR) install

dist:
	$(MAKE) -C $(SRC_DIR) dist
