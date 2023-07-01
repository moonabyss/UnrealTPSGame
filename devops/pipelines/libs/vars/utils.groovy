def SlackColorFromBuildResult (result) {
    if ("${result}" == 'SUCCESS') return '#18a558';
    if ("${result}" == 'ABORTED') return '#6e6e6e';
    if ("${result}" == 'UNSTABLE') return '#fcba03';
    return '#ff0000';
}