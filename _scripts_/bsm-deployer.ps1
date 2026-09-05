# PowerShell Script

# Bismuth Compiler Deployer Script 

# Questo script permette l'automazione di alcune azioni di sviluppo del progetto bismuth Compiler

param (
	[string]$FunctionName #funzione richiamata
)

echo "--------------------------------"
echo "Bismuth Compiler Deployer Script"
echo "--------------------------------"

if([string]::IsNullOrWhiteSpace($FunctionName)) {
	echo "usage: .\script <function-name>"
	echo ""
	return
}

# Funzioni helper
# Queste funzioni corispondono ai comandi disponibili

function help_func 
{
	echo ""
	echo "Help Section"
	echo "------------"
	echo "This script allows to execute automatically some actions for the bismuth compiler project deployment"
	echo "To select an action to execute: use: .\script <function-name>"
	echo "To see the available functions of this script:  use: .\script list"
	echo ""
}

$list_cmds = $false
function list_func {
	$script:list_cmds = $true
}

function deploy_func 
{
	# 1. cerca la cartella del progetto Bismuth
	
	echo "searching for bismuth compiler project directory..."
	
	$bsm_folder = Get-ChildItem -Path "C:/users/utente/desktop" -Directory -Recurse -Filter "MyCompiler" | Select-Object -First 1
	
	if(-not $bsm_folder) {
		echo "could not find bismuth project directory."
		return
	}
	echo "project directory found."
	
	# 2. carca il file cmakelists
	
	echo " " 
	echo "searching for cmakelists file..."
	
	$cmakelists = Get-ChildItem -Path $bsm_folder.FullName -Recurse -Filter "CMakeLists.txt" | Select-Object -First 1
	
	if(-not $cmakelists) {
		echo "cmakelists file not found."
		return
	}
	echo "cmakelists file found."
	
	# 3. Estrazione di version dal cmakelists
	
	echo " "
	echo "exctracting latest version number from cmake file..."
			
	$content = Get-Content $cmakelists.FullName -Raw 
	
	if ($content -match '(?m)^\s*VERSION\s+([0-9]+\.[0-9]+\.[0-9]+)') {
		$version = $matches[1]
	} else {
		echo "version number not found."
		return
	}
	echo "version number found."
	
	# 4. Cerca la directory bin
	
	echo ""
	echo "searching for bin directory..."
	
	$bin = Get-ChildItem -Path $bsm_folder.FullName -Directory -Recurse -Filter "bin" | Select-Object -First 1
	
	if(-not $bin) {
		echo "bin directory not found."
		return
	}
	echo "bin directory found."
	
	# 5. Cerca la folder di release
	
	echo ""
	echo "searching for release folder in project..."
	
	$releases = Get-ChildItem -Path $bsm_folder.FullName -Directory -Recurse -Filter "releases" | Select-Object -First 1
	
	if(-not $releases) {
		echo "release folder not found."
		return
	}
	echo "release folder found."
	
	# 6. Copia di bin in releases + rename di bin
	
	echo ""
	echo "moving bin directory to release staging..."
	
	#nuova folder per la release e poi ci si copia dentro bin
	$f = New-Item -Path $releases.FullName -Name ("v" + $version) -ItemType Directory -ErrorAction Stop
	Copy-Item -Path $bin.FullName -Destination $f.FullName -Recurse -ErrorAction Stop
	
	echo "bin directory correctly moved."
	
	$copied_bin = Join-Path $f.FullName $bin.Name #cartella bin copiata in releases
						
	$release_name = ("bismuth-v"+$version+"-win64") 
						
	echo ""
	echo "renaming bin into:" $release_name
						
	#rinominare bin in bimsuth-vVersion-win64
	Rename-Item -Path $copied_bin -NewName $release_name -ErrorAction Stop
						
	echo "bin correctly renamed to release package."
	
	# 7. Zip del package di release
	
	echo ""
	echo "searching for 7z..."
	
	$seven_zip = (Get-Command 7z).Source
						
	if(-not $seven_zip) {
		echo "7z program not found."
		return
	}
						
	echo "7z program found."
						
	#programma con 7z (a: archive, -mmt: cpu threads, -mx9: ultra compression)
	echo ""
	echo "adding package to archive..."
						
	$release_folder = Join-Path $f.FullName $release_name
	$archive = Join-Path $f.FullName ($release_name + ".zip")
						
	7z a $archive $release_folder -tzip -mmt=3 -mx=9
	
	echo ""
	echo "compressed release package. done."
	
	return
}

# Mappa nome-funzione -> Function

$commands = @{
	"help" = ${function:help_func}
	"list" = ${function:list_func}
	"deploy" = ${function:deploy_func}
}

# Switch principale che richiama le funzioni helper (nomi dei comandi hardcoded)

if($commands.ContainsKey($FunctionName)) {
	& $commands[$FunctionName] # & permette di eseguire l'oggetto FunctionInfo contenuto nella mappa
} else {
	echo "command not found. type 'help' for info."
	echo ""
}

if($list_cmds) {
	echo $commands.Keys
	return 
}