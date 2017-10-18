if [ ! -d ~/edgent-1.1.0/ ]; then
	SUCCESSFUL_DOWNLOAD=false

	edgent_binary="apache-edgent-1.1.0-incubating-bin.tgz"

	# ==================== Downloading binary file ====================

	if [ ! -f $edgent_binary ]; then
	    echo ">> Downloading Edgent 1.1.0 binary file..."

	    mirrors=(
	        "http://mirror.nbtelecom.com.br/apache/incubator/edgent/1.1.0-incubating/binaries"
	        "http://ftp.unicamp.br/pub/apache/incubator/edgent/1.1.0-incubating/binaries"
	        "http://mirror.nbtelecom.com.br/apache/incubator/edgent/1.1.0-incubating/binaries"
	        "http://www-eu.apache.org/dist/incubator/edgent/1.1.0-incubating/binaries"
	        "http://www-us.apache.org/dist/incubator/edgent/1.1.0-incubating/binaries"
	    )

	    for mirror in "${mirrors[@]}"; do
	        wget $mirror/$edgent_binary

	        if [ $? -eq 0 ]; then
	            SUCCESSFUL_DOWNLOAD=true
	            break
	        fi
	    done

	    if ! $SUCCESSFUL_DOWNLOAD; then
	    	echo ">> ERROR: Failed downloading $edgent_binary. Reverting changes and exiting!"
	    	rm -f $edgent_binary
	    	exit 1
	    fi
	else
		echo ">> The $edgent_binary file already exists. Moving on..."
	fi

	main_distrubuition_site="https://www.apache.org/dist/incubator/edgent/1.1.0-incubating/binaries"

	# ==================== Checking release manager signature ====================

	key_file="KEYS"

	if [ ! -f $key_file ]; then
		echo ">> Downloading $key_file..."

		wget "https://www.apache.org/dist/incubator/edgent/$key_file"

	    if [ $? -ne 0 ]; then
	    	echo ">> ERROR: Failed downloading $key_file. Reverting changes and exiting!"
	    	if $SUCCESSFUL_DOWNLOAD; then
	    		rm -f $edgent_binary
	    	fi
	    	rm -f $key_file
	    	exit 1
	    fi
	else
		echo ">> The $key_file file already exists. Moving on..."
	fi

	edgent_asc="$edgent_binary.asc"

	if [ ! -f $edgent_asc ]; then
		echo ">> Downloading $edgent_asc..."

		wget $main_distrubuition_site/$edgent_asc

	    if [ $? -ne 0 ]; then
	    	echo ">> ERROR: Failed downloading $edgent_asc. Reverting changes and exiting!"
	    	if $SUCCESSFUL_DOWNLOAD; then
	    		rm -f $edgent_binary
	    	fi
	    	rm -f $key_file
	    	rm -f $edgent_asc
	    	exit 1
	    fi
	else
		echo ">> The $edgent_asc file already exists. Moving on..."
	fi

	echo ">> Verifying $edgent_binary signature..."

	gpg --import $key_file

    if [ $? -ne 0 ]; then
    	echo ">> ERROR: Failed importing key. Reverting changes and exiting!"
    	if $SUCCESSFUL_DOWNLOAD; then
    		rm -f $edgent_binary
    	fi
    	rm -f $key_file
    	exit 1
    fi

	gpg --verify $edgent_asc $edgent_binary

    if [ $? -ne 0 ]; then
    	echo ">> ERROR: Failed verifying signature. Reverting changes and exiting!"
    	if $SUCCESSFUL_DOWNLOAD; then
    		rm -f $edgent_binary
    	fi
    	rm -f $key_file
    	rm -f $edgent_asc
    	exit 1
    fi

	echo ">> Good signature!"
	rm -f $key_file
	rm -f $edgent_asc

	# ==================== Checking that a file has been downloaded correctly ====================

	edgent_md5="$edgent_binary.md5"

	if [ ! -f $edgent_md5 ]; then
		echo ">> Downloading $edgent_md5..."

		wget $main_distrubuition_site/$edgent_md5

	    if [ $? -ne 0 ]; then
	    	echo ">> ERROR: Failed downloading $edgent_md5. Reverting changes and exiting!"
	    	if $SUCCESSFUL_DOWNLOAD; then
	    		rm -f $edgent_binary
	    	fi
	    	rm -f $edgent_md5
	    	exit 1
	    fi
	else
		echo ">> The $edgent_md5 file already exists. Moving on..."
	fi

	echo ">> Verifying $edgent_binary hash..."

	tgz_file="`md5sum $edgent_binary`"
	md5_file="`cat $edgent_md5`  $edgent_binary"

	if [[ $tgz_file != $md5_file ]]; then
	    echo ">> ERROR: MD5 sums mismatch. Reverting changes and exiting!"
		if $SUCCESSFUL_DOWNLOAD; then
			rm -f $edgent_binary
		fi
		rm -f $edgent_md5
		exit 1
	fi

	echo ">> MD5 cecksums OK!"
	rm -f $edgent_md5

	# ==================== Installing Edgent ====================

	echo ">> Extracting $edgent_binary to home directory..."
    tar -xzf $edgent_binary -C ~

    echo ">> Cleaning up..."
    rm -f $edgent_binary

    echo ">> Done!"
else
    echo ">> The ~/edgent-1.1.0/ directory already exists. Done!"
fi